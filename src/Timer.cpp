#include "Timer.hpp"

Timer::Timer() {
	m_Time = 0.0f;
}

Timer::~Timer() {
	m_Time = 0.0f;
}

void Timer::Set(float time) {
	m_Time = time;
}

void Timer::Move(float dt) {
	if(m_Time >= 0.0f) m_Time -= dt;
}

bool Timer::Finished() const {
	return m_Time <= 0.0f;
}


TimerD::TimerD() {
	m_Time = 0.0;
}

TimerD::~TimerD() {
	m_Time = 0.0;
}

void TimerD::Set(double time) {
	m_Time = time;
}

void TimerD::Move(float dt) {
	if(m_Time >= 0.0) m_Time -= dt;
}

bool TimerD::Finished() const {
	return m_Time <= 0.0;
}
