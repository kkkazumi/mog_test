#include <deque>
template<class InputType, class InnerAvgType=double,
    class AvgType=InputType>
class MovingAverage {
    int average_count;
    std::deque<InputType> buffer;
    InnerAvgType average;
public:
    MovingAverage() : average_count(1), average(0) { }
    MovingAverage(int average_count)
        : average_count(average_count), average(0)
    {
        assert(average_count > 0);
    }
     
    int getAverageCount() const {
        return average_count;   
    }
    void setAverageCount(int n) {
        assert(n > 0);
        average_count = n;
        reset();
    }
     
    // reset average value and counter.
    void reset() {
        average = 0;
        buffer.clear();
    }
    // average value stability
    bool isStable() const {
        return buffer.size() >= average_count;
    } 
    // update and return average value
    AvgType update(const InputType& input) {
        InnerAvgType total = average * buffer.size();
        if(buffer.size() >= average_count) {
            total -= buffer.at(0);
            buffer.pop_front();
        }
        buffer.push_back(input);
        total += (InnerAvgType)input;
        average = total / buffer.size();
        return (AvgType)average;
    }
    operator AvgType () {
        return (AvgType)average;
    }
};
