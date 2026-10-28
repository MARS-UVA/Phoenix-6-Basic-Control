#include <ctre/phoenix6/TalonFX.hpp>
#include <ctre/phoenix6/unmanaged/Unmanaged.hpp>
#include <chrono>
#include <thread>
#include <ncurses.h>

using namespace ctre::phoenix6;

static constexpr CANBus CANBUS{"*"};

int main(void) {
    hardware::TalonFX motor{33, CANBUS};
    controls::DutyCycleOut motor_control{0};

    hardware::TalonFX motor_2{34, CANBUS};
    controls::DutyCycleOut motor_2_control{0};

    configs::TalonFXConfiguration fx_cfg{};

    fx_cfg.MotorOutput.Inverted = signals::InvertedValue::CounterClockwise_Positive;
    motor.GetConfigurator().Apply(fx_cfg);
    motor_2.GetConfigurator().Apply(fx_cfg);

    double speed = 0.3;

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
                motor_control.Output = speed;
		last_input = now;
		break; 
	    case 's':
                motor_control.Output = speed;
		last_input = now;
		break; 
	    case 'a':
                motor_control.Output = speed;
		last_input = now;
		break; 
	    case 'd':
		motor_control.Output = speed;
		last_input = now;
		break; 
	    case ' ':
		motor_control.Output = 0.0;
		break;
	    case 'q':
		endwin();
		return 0;
	}

	if (now - last_input > std::chrono::milliseconds(500)) {
		motor_control.Output = 0.0;
	}

	ctre::phoenix::unmanaged::FeedEnable(10);
	motor.SetControl(motor_control);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
