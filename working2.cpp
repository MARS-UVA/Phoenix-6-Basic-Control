#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/unmanaged/Unmanaged.hpp>
#include <chrono>
#include <thread>
#include <ncurses.h>

using namespace ctre::phoenix6;

static constexpr CANBus CANBUS{"*"};

int main(void) {
    hardware::TalonFX right_leader{33, CANBUS};
    hardware::TalonFX left_leader{34, CANBUS};

    controls::DutyCycleOut right_control{0};
    controls::DutyCycleOut left_control{0};

    configs::TalonFXConfiguration fx_cfg{};
    fx_cfg.MotorOutput.Inverted = signals::InvertedValue::CounterClockwise_Positive;
    right_leader.GetConfigurator().Apply(fx_cfg);
    left_leader.GetConfigurator().Apply(fx_cfg);

    double speed = 0.1;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    refresh();

    using Clock = std::chrono::steady_clock;
    auto last_input = Clock::now();

    while (true) {
	int key = getch();
	auto now = Clock::now();

	switch (key) {
	    case 'w':
                right_control.Output = speed;
		left_control.Output = speed;
		last_input = now;
		break; 
	    case 's':
                right_control.Output = speed * -1;
		left_control.Output = speed * -1;
		last_input = now;
		break; 
	    case 'a':
                right_control.Output = speed;
		left_control.Output = speed * -1;
		last_input = now;
		break; 
	    case 'd':
		right_control.Output = speed * -1;
		left_control.Output = speed;
		last_input = now;
		break;
	    case '1':
		speed = 0.1;
		break;
	    case '2':
		speed = 0.2;
		break;
	    case '3':
		speed = 0.3;
		break;
	    case '4':
		speed = 0.4;
		break;
	    case '5':
		speed = 0.5;
		break;
	    case '6':
    		speed = 0.6;
		break;
	    case '7':
		speed = 0.7;
		break;
	    case '8':
		speed = 0.8;
		break;
	    case '9':
		speed = 0.9;
		break;
	    case '0':
		speed = 1.0;
		break;		
	    case ' ':
		right_control.Output = 0.0;
		left_control.Output = 0.0;
		break;
	    case 'q':
		endwin();
		return 0;
	}

	if (now - last_input > std::chrono::milliseconds(500)) {
		right_control.Output = 0.0;
		left_control.Output = 0.0;
	}

	ctre::phoenix::unmanaged::FeedEnable(10);
	right_leader.SetControl(right_control);
	left_leader.SetControl(left_control);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
