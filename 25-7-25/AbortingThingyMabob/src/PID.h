#ifndef PID_H
#define PID_H

class PID {
public:
    PID();
    PID(float kp, float ki, float kd);

    void setP(float);
    void setI(float);
    void setD(float);
    void setTunings(float kp, float ki, float kd);
    void setOutputLimits(float min, float max);
    void setSampleTime(unsigned long newSampleTime);

    float compute(float setpoint, float input);

    void reset();

private:
    float kp, ki, kd;
    float lastInput;
    float integral;
    float minOutput, maxOutput;

    unsigned long lastTime;
    unsigned long sampleTime; // in milliseconds
};

#endif
