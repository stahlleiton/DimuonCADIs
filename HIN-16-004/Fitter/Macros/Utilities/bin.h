#ifndef bin_h
#define bin_h

#include <utility>
#include <tuple>
#include <iostream>
#include <set>

using namespace std;

// a simple template class to store a bin and overload the equality operator

// define a few common uses of the template class
template <typename T> class bin : public pair<T,T> {
   public:
      bin(T a, T b) : pair<T,T>(a,b) {};
      T low() const {return this->first;}
      T high() const {return this->second;}
};
typedef bin<double> binD;
typedef bin<float> binF;
typedef bin<int> binI;

// associate three such bins to make an analysis bin
class anabin : public tuple<binF,binF,binI> {
   public:
      anabin(float rapmin, float rapmax, float ptmin, float ptmax, int centmin, int centmax) :
         tuple<binF,binF,binI> (binF(rapmin,rapmax), binF(ptmin, ptmax), binI(centmin, centmax)) {};
      binF rapbin() const {return get<0>(*this);};
      binF ptbin() const {return get<1>(*this);};
      binI centbin() const {return get<2>(*this);};
      void setrapbin(binF rapbin) {get<0>(*this) = rapbin;};
      void setptbin(binF ptbin) {get<1>(*this) = ptbin;};
      void setcentbin(binI centbin) {get<2>(*this) = centbin;};
      void print() const {
         cout << "rap=[" << get<0>(*this).low() << "," << get<0>(*this).high() <<
            "], pt=[" << get<1>(*this).low() << "," << get<1>(*this).high() <<
            "], cent=[" << get<2>(*this).low() << "," << get<2>(*this).high() << "]" << endl;
      }
};

set<anabin> allbins() {
   set<anabin> ans;
   ans.insert(anabin(0,1.6,6.5,30,0,200));
   ans.insert(anabin(0,1.6,6.5,30,0,20));
   ans.insert(anabin(0,1.6,6.5,30,20,40));
   ans.insert(anabin(0,1.6,6.5,30,40,60));
   ans.insert(anabin(0,1.6,6.5,30,60,80));
   ans.insert(anabin(0,1.6,6.5,30,80,100));
   ans.insert(anabin(0,1.6,6.5,30,100,200));
   ans.insert(anabin(0,1.6,6.5,9,0,200));
   ans.insert(anabin(0,1.6,9,12,0,200));
   ans.insert(anabin(0,1.6,12,15,0,200));
   ans.insert(anabin(0,1.6,15,20,0,200));
   ans.insert(anabin(0,1.6,20,30,0,200));
   ans.insert(anabin(1.6,2.4,3,30,0,200));
   ans.insert(anabin(1.6,2.4,3,30,0,40));
   ans.insert(anabin(1.6,2.4,3,30,40,80));
   ans.insert(anabin(1.6,2.4,3,30,80,200));
   ans.insert(anabin(1.6,2.4,3,6.5,0,200));
   ans.insert(anabin(1.6,2.4,6.5,12,0,200));
   ans.insert(anabin(1.6,2.4,12,30,0,200));
   return ans;
};

#endif // #ifndef bin_h
