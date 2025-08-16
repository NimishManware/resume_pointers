#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;



class Point {
private:
	float x;
	float y;

public:
	Point()
		: x(0), y(0) {}  // Default constructor initializing coordinates to (0,0)
	Point(float xVal, float yVal)
		: x(xVal), y(yVal) {}  // Parameterized constructor

	float X() const { return x; }  // Accessor for x coordinate
	float Y() const { return y; }  // Accessor for y coordinate

};
float find_length(vector<pair<Point, Point> >& posters,int n){
	float length = 0;
	float start = posters[0].first.X();
	float end = posters[0].second.X();

	for(int i = 1; i < n; i++){
		if(posters[i].first.X() <= end){
			end = max(end,posters[i].second.X());
			if (i == n - 1){
				length += end - start;
			}
		}
		else{
			length += end - start;
			start = posters[i].first.X();
			end = posters[i].second.X();
			if(i == n - 1){
				length += end - start;
			}
		}
	}
	return length;
}
void print_outline(vector<Point>& outline){
	int n = outline.size();
	for (int i = 0; i < n; i++){
		std::cout << outline[i].X() << " " << outline[i].Y() << "\n";
	}
}
float area_from_outline(vector<Point> outline){
    float area;
    int n = outline.size();
    for(int i = 0 ;i < n -1;i++){
        if(outline[i].Y() == 0 && outline[i+1].Y() == 0) continue;
        float base = outline[i+1].X() - outline[i].X();
        float height = 0.5 *(outline[i+1].Y() + outline[i].Y());
        area += base * height; 
    }
    return area;
}
Point inter(pair<Point,Point> seg1,pair<Point,Point> seg2){
	float m1,m2;
	if(seg1.first.X() - seg1.second.X())
		m1 = (seg1.first.Y() - seg1.second.Y())/
						(seg1.first.X() - seg1.second.X());
	else{ 
		m1 = INT_MAX;
	}
	if(seg2.first.X() - seg2.second.X())
		m2 = (seg2.first.Y() - seg2.second.Y())/
						(seg2.first.X() - seg2.second.X());
	else 
		m2 = INT_MAX;
	float x,y;

	x = seg2.first.Y() - seg1.first.Y() + m1 * seg1.first.X() - m2 * seg2.first.X();
	x = x /(m1 - m2);
	if(m1 != INT_MAX)
		y = seg1.first.Y() + x * m1 - m1 * seg1.first.X();
	else
		y = seg2.first.Y() + x * m2 - m2 * seg2.first.X();
	return Point(x,y);
}
vector<Point> outline_merge(vector<Point>& outline1, vector<Point>& outline2){
	float start = outline2[0].X();
	int i;
	int k = 0;
	int j = 0;
	int n1 = outline1.size();
	int n2 = outline2.size();
	vector<Point> outline;
	for(i = 0 ; i < n1; i ++){
		if(outline1[i].X() >= start){
			break;
		}
		outline.push_back(outline1[i]);
		k++;
	}
	if(i == n1){
		for(; k < n1 + n2 ; k++){
			outline.push_back(outline2[j]);
			j++;
		}
		return outline;
	}
	bool ahead = true;
    bool above = outline1[i-1].Y() >= 0;
	float y1,y2;
	while(i < n1 && j < n2){
		if(ahead){
			y1 = outline1[i-1].Y(); 
			y1 += (outline1[i].Y() - outline1[i-1].Y())*
							(outline2[j].X() - outline1[i-1].X())/
										(outline1[i].X() - outline1[i-1].X());
			y2 = outline2[j].Y();
			if (above != y1 >= y2){
				//intersection
				above = !above;
				if (j)
				outline.push_back(inter(make_pair(outline1[i-1],outline1[i]),
									make_pair(outline2[j-1],outline2[j])));
				else
				outline.push_back(inter(make_pair(outline1[i-1],outline1[i]),
									make_pair(Point(outline2[j].X(),0),outline2[j])));
				k++;
			}
			if(!above){
				outline.push_back(outline2[j]);
				k++;
			}
			j++;	
			ahead = outline1[i].X() >= outline2[j].X();
		}else{
			y2 = outline2[j-1].Y(); 
			y2 += (outline2[j].Y() - outline2[j-1].Y())*
							(outline1[i].X() - outline2[j-1].X())/
										(outline2[j].X() - outline2[j-1].X());
			y1 = outline1[i].Y();
			if (above != y1 >= y2){
				//intersection
				above = !above;
				if (j)
				outline.push_back(inter(make_pair(outline1[i-1],outline1[i]),
									make_pair(outline2[j-1],outline2[j])));
				else
				outline.push_back(inter(make_pair(outline1[i-1],outline1[i]),
									make_pair(Point(outline2[j].X(),0),outline2[j])));
				k++;
			}
			if(above){
				outline.push_back(outline1[i]);
				k++;
			}
			i++;
			ahead = outline1[i].X() >= outline2[j].X();
		}
	}
	while (j < n2) {
		outline.push_back(outline2[j]);
		j++;
	}
	while (i < n1) {
		outline.push_back(outline1[i]);
		i++;
	}
	return outline;
}
vector<Point> find_outline(vector <pair<Point, Point> >& posters,int low,int high){
	// std::cout<<low<<" "<<high<<"\n";
	if(low < high){
		int mid = low + (high - low)/2;
		vector<Point> a = find_outline(posters,low,mid);
		vector<Point> b = find_outline(posters,mid+1,high);
		return outline_merge(a,b);
	}
	else {
		//case for single trapezium
		vector<Point> outline; //scope for opt
		if(posters[low].first.Y()){
			outline.push_back(Point(posters[low].first.X(),0));
			outline.push_back(posters[low].first);
			outline.push_back(posters[low].second);
			if(posters[low].second.Y()){
				outline.push_back(Point(posters[low].second.X(),0));
			}
		}
		else{
			outline.push_back(posters[low].first);
			outline.push_back(posters[low].second);
			if(posters[low].second.Y()){
				outline.push_back(Point(posters[low].second.X(),0));
			}
		}
		return outline;
	}
}
// int find_area(vector <pair<Point, Point> > posters,int n){

// }
void merge(vector<pair<Point, Point> >& posters,int low,int mid,int high){
	int n1 = mid - low + 1;
    int n2 = high - mid;

    vector<pair<Point, Point> > L(n1);
    vector<pair<Point, Point> > R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = posters[low + i];
    for (int j = 0; j < n2; j++)
        R[j] = posters[mid + 1 + j];

    int i = 0, j = 0, k = low;
    while (i < n1 && j < n2) {
        if (L[i].first.X() <= R[j].first.X()) {
            posters[k] = L[i];
            i++;
        } else {
            posters[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        posters[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        posters[k] = R[j];
        j++;
        k++;
    }
}


void merge_sort(vector<pair<Point, Point> >& posters,int low,int high){
	if(low < high){
		int mid = low + (high - low)/2;

		merge_sort(posters,low,mid);
		merge_sort(posters,mid+1,high);

		merge(posters,low,mid,high);
	}
}

int main(){
	   /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
	int n;
	cin >> n;
	float a,b,c,d;

	vector <pair<Point, Point> > posters; 
	
	for(int i=0; i<n; i++){
		cin >> a;
		cin >> b;
		cin >> c;
		cin >> d;

		posters.push_back(make_pair(Point(a,b), Point(c,d)));	
	}
	
	merge_sort(posters,0,n-1);
	
	float lengthCovered = find_length(posters,n); 
	
	vector<Point> outline = find_outline(posters,0,n-1);

	float area =area_from_outline(outline);
 
	std::cout << static_cast<int>(lengthCovered)<< endl;
	
	std::cout << static_cast<int>(area)<< endl;

	return 0;
}

