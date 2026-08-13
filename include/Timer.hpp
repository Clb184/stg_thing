#ifndef TIMER_INCLUDED
#define TIMER_INCLUDED

class Timer {
public:
	Timer();
	~Timer();
	
	void Set(float time);
	void Move(float dt);
	bool Finished() const;

private:
	float m_Time;

};


class TimerD {
public:
	TimerD();
	~TimerD();
	
	void Set(double time);
	void Move(float dt);
	bool Finished() const;

private:
	double m_Time;

};

#endif
