from scipy.fft import fft, fftfreq
import matplotlib.pyplot as plt
from scipy.signal import stft
import numpy as np
import pyaudio
import wave
from timeit import default_timer as timer
import generator as gen

# Parameters for recording
FORMAT = pyaudio.paInt16  
CHANNELS = 1  
RATE = 88200 
CHUNK = 1024  
BIT_INTERVAL = 0.3
THRESHOLD = 0.0005   
GENERATOR = "010111010111"
FILTER_1 = 11000
FILTER_0 = 5000

#For CSMA Protocol
DIFS = 5
SIFS = 2
N = 1
MAX_WAIT = 110 * BIT_INTERVAL
ACK_WAIT = 32 * BIT_INTERVAL
MAC = 1 #<= edit MAC

#utility for string to number
def strToDec(s,nbits):
     ans = 0
     s = s[::-1]
     for i in range(nbits):
         if(s[i] == '1'):
             ans += (1 << i)
     return ans

#send ACK
def sendACK(senderMAC , recieverMAC):
    print("Sending ACK ...")
    finalMessage = senderMAC + recieverMAC
    gen.sendMsg(finalMessage)
    print("Sent ACK ...")

#waitACK
def waitACK(dest_MAC, waitTime):
    global N
    #opening stream
    audio = pyaudio.PyAudio()
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)
    start = timer()
    print("Waiting for ACK ...")
    received_message = ""

    #collecting frames
    frames = []
    for _ in range(0, int(RATE / CHUNK * waitTime)):
        data = stream.read(CHUNK)
        frames.append(data)
    end = timer()
    print("Waiting time over")

    #closing stream
    stream.stop_stream()
    stream.close()
    audio.terminate()
    
    #normalize waveform
    waveform = np.frombuffer(b''.join(frames), dtype=np.int16)
    waveform = waveform / 32767.0 
    
    #fft analysis
    frequencies, times, Zxx = stft(waveform, fs=RATE, nperseg=1024, noverlap=512)

    #analysing frequencies
    for i, time in enumerate(times):
        if np.isclose(time % BIT_INTERVAL, 0, atol= 512/RATE): 
            magnitudes = np.abs(Zxx[:, i])
            detected_indices = np.where(magnitudes > THRESHOLD)[0]
            if detected_indices.size > 0:
                detected_freqs = frequencies[detected_indices]
                max_freq = np.max(detected_freqs)
            else:
                max_freq = 0 

            if max_freq > FILTER_1:
                received_message += "1"
            elif max_freq > FILTER_0:
                received_message += "0"
            else :
                #x represents return to zero
                received_message += "x"

    ACK = strip_and_remove_x(received_message)
    print("ACK")
    #checking ACK format
    if len(ACK) == 4:
        sender = ACK [0:2]
        receiver = ACK[2:]
        if not (receiver == gen.decTobitstring(MAC,2) and sender == gen.decTobitstring(dest_MAC,2)):
            N = N + 1
            return False
        return True
    return False    
    
    
#carrier sense
def carrierSense(waitTime):
    global N
    #opening stream
    audio = pyaudio.PyAudio()
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)
    start = timer()
    frames = []
    #real time analysis
    for _ in range(0, int(RATE / CHUNK * waitTime)):
        data = stream.read(CHUNK)
        frames.append(data)
        now = timer()
        if now - start > BIT_INTERVAL:
            start = np.floor(now * 10) / 10
            combined_data = b''.join(frames)
            frames = []
            audio_data = np.frombuffer(combined_data, dtype=np.int16)
            audio_data = audio_data / 32767.0
            f, t, Zxx = stft(audio_data, fs=RATE, nperseg= 1024, noverlap=512)
            magnitudes = np.abs(Zxx[:,-1])
            detected_indices = np.where(magnitudes > THRESHOLD)[0]
            if detected_indices.size > 0:
                detected_freqs = f[detected_indices]
                max_freq = np.max(detected_freqs)
            else:
                max_freq = 0
            if max_freq > FILTER_1:
                print(max_freq)
                print("Carrier is busy. Received a 1")
                N = N + 1
                return True
            elif max_freq > FILTER_0:
                print(max_freq)
                print("Carrier is busy. Received a 0")
                N = N + 1
                return True
            else:
                print(max_freq)
                print("x")
            
    print("Channel clear")
    #closing stream
    stream.stop_stream()
    stream.close()
    audio.terminate()
    return False
            

#listen function
def listenMsg(waitTime = MAX_WAIT):
    #opening stream
    audio = audio = pyaudio.PyAudio()
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                        rate=RATE, input=True,
                        frames_per_buffer=CHUNK)
    start = timer()
    received_message = ""
    #collecting frames
    frames = []
    for _ in range(0, int(RATE / CHUNK * waitTime)):
        data = stream.read(CHUNK)
        frames.append(data)
    end = timer()
    #closing stream
    stream.stop_stream()
    stream.close()
    audio.terminate()
    
    #normalize waveform
    waveform = np.frombuffer(b''.join(frames), dtype=np.int16)
    waveform = waveform / 32767.0 
    
    #fft analysis
    frequencies, times, Zxx = stft(waveform, fs=RATE, nperseg=1024, noverlap=512)

    #analysing frequencies
    for i, time in enumerate(times):
        if np.isclose(time % BIT_INTERVAL, 0, atol= 512/RATE): 
            magnitudes = np.abs(Zxx[:, i])
            detected_indices = np.where(magnitudes > THRESHOLD)[0]
            if detected_indices.size > 0:
                detected_freqs = frequencies[detected_indices]
                max_freq = np.max(detected_freqs)
            else:
                max_freq = 0 

            if max_freq > FILTER_1:
                received_message += "1"
            elif max_freq > FILTER_0:
                received_message += "0"
            else :
                received_message += "x"

    return received_message, end
             
#removing return to zero character
def strip_and_remove_x(bitstring):
    stripped_bitstring = bitstring.strip('x')
    cleaned_bitstring = stripped_bitstring.replace('x', '')
    return cleaned_bitstring

#extract info from the recieved message
def getInfo(received_message):
    received_message = strip_and_remove_x(received_message)
    n = len(received_message)
    i = 0
    while(i<n):
        if(received_message[i : i+8] == '10101111'):
            break
        else:
            i += 1
    
    lenBits = received_message[i + 8 : i + 13]
    lengthOfMessage = strToDec(lenBits,5) - 4
    senderMAC = received_message[i+13 : i+15]
    receiverMAC = received_message[i+15 : i+17]
    sentMessage = received_message[i+17 : i+17 + lengthOfMessage]

    isMyMsg = True
    collision = False

    if(strToDec(receiverMAC,2) != MAC):
        isMyMsg = False
    if(gen.mod2div(sentMessage,GENERATOR) != "0" * (len(GENERATOR) - 1)):
        collision = True

    return {"isMyMsg" : isMyMsg , "collision" : collision , "lenBits" : lenBits , "lenMsg" : lengthOfMessage , "senderMAC" : senderMAC , "recieverMAC" : receiverMAC , "sentMsg" : sentMessage}
