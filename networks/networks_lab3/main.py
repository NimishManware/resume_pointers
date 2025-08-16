import keyboard,random
import generator as gen
import recording as rec
from timeit import default_timer as timer


# message => frame
def frameGenerator(message):
    msgString = message["msg"]
    destMAC = message["destMAC"]
    crcPlusMessage = gen.encode_data(msgString)
    finalMsg = gen.addPreamble(crcPlusMessage , rec.MAC , destMAC)
    return finalMsg

# input queue
InitQueue = []
SendQueue = []

for i in range(2):
    message = input(f"Enter message {i + 1}: ")
    parsedMsg = message.split()
    msgString = parsedMsg[0]
    destMAC = int(parsedMsg[1])
    if destMAC == -1 :
        #ignore this input
        continue
    else:
        InitQueue.append({"msg": msgString, "destMAC": destMAC}) 

# Function to handle triggers
def on_key_event(e):
    if e.name in ['enter', 'return']: 
        if len(InitQueue) > 0:
            msg = InitQueue.pop(0) 
            msgString = msg["msg"]
            dest_MAC = msg["destMAC"]
            SendQueue.append(msg)  
            print(f"Message added to send queue : {msgString} to {dest_MAC}")
        else:
            print("You sent all the messages")
                
keyboard.on_press(on_key_event)

#CSMA loop
while True:
    waitTime = rec.DIFS + random.randrange(0,2 ** rec.N) * rec.BIT_INTERVAL
    #carrier sensing
    carrier_busy = rec.carrierSense(waitTime) 
    #carrier is busy
    if(carrier_busy): 
        #trying to listen
        msg , endTime = rec.listenMsg(rec.MAX_WAIT) 
        info = rec.getInfo(msg) 
        #checking whether message is meant for me and no collision occurred
        if((info["isMyMsg"]) and (not info["collision"])):
            print("[RECVD]: ", info["sentMsg"][:-11] , info["senderMAC"] , endTime)
            #send  the acknowledgement
            rec.sendACK(info["recieverMAC"],info["senderMAC"])

    elif(len(SendQueue) == 0):
        #nothing to send 
        continue  
    else:
        #send the top of send queue
        msg = SendQueue[0]
        msgString = msg["msg"]
        destMAC = msg["destMAC"]
        #broadcasting
        if(destMAC == 0):
            for j in range(1 , 3):
                if(j != rec.MAC):
                    msg["destMAC"] = j
                    gen.sendMsg(frameGenerator(msg))
                    break
        #normal sending
        else:
             gen.sendMsg(frameGenerator(msg))

        #waiting for acknowledgement
        sent = rec.waitACK(msg["destMAC"],rec.ACK_WAIT)
        if sent:
            #if got the ACK then pop from sendQueue
            SendQueue.pop(0)
            sendTime = timer()
            print("[SENT]: ", msgString , destMAC , sendTime)
        else:
            #ACK not recieved
            print("Transmission failed. Trying again after some time")
            continue
