#ifndef REQUEST_H
#define REQUEST_H

class Request
{
private:
    double generateTime;
public:
    Request() : generateTime(0.0) {}
    Request(double generateTime) {this->generateTime = generateTime;}
    Request(const Request& request)
    {
        this->generateTime = request.generateTime;
    }
    Request operator=(const Request& request)
    {
        this->generateTime = request.generateTime;
        return *this;
    }
    ~Request() {}
    double GetGeneratedTime() const {return generateTime;}
    void SetGeneratedTime(double generateTime) {this->generateTime = generateTime;}
};

#endif // REQUEST_H
