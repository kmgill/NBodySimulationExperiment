/*
 * gtkmain.cpp
 *
 *  Created on: Apr 29, 2015
 *      Author: kgill
 */

#include "common.h"
#include "init.h"
#include "leapfrogsimulator.h"
#include "collisionproviders.h"
#include <vector>
#include <iostream>
#include <omp.h>
#include <pthread.h>
#include "mathext.h"

#if USING_GTK_MAIN

// Basic configuration
#define NBODY_ITERATION_INTERVAL         1.0
#define NBODY_NUM_ITERATIONS             (86400 * 100)
//* 365 * 1)
#define NBODY_CHECK_COLLISIONS           false
#define NBODY_ALLOW_MAJOR_BODIES         true
#define NBODY_ALLOW_MINOR_BODIES         true
#define NBODY_ALLOW_MOONS                true
#define NBODY_SET_CPU_AFFINITY           false

#include <gtkmm.h>

using namespace std;


class SimulationDisplay : public Gtk::DrawingArea {

public:
	SimulationDisplay() {

	}
	virtual ~SimulationDisplay() {

	}


};

/* Poorly designed and constructed window
 *
 */
class SimulationApp {
protected:
	apoapsys::LeapFrogSimulator * m_simulator;
	real m_interval;
	bool m_paused;
	bool m_checkingCollisions;

	SimulationDisplay * m_drawingArea;
	Gtk::Button * m_buttonPause;
	Gtk::CheckButton * m_checkCollisions;

	void onCollisionsCheck() {
		m_checkingCollisions = m_checkCollisions->get_active();
	}

	void onPauseButton() {
		if (this->m_paused) {
			this->m_buttonPause->set_label("Pause");
		} else {
			this->m_buttonPause->set_label("Unpause");
		}
		this->m_paused = !this->m_paused;
	}

public:
	Gtk::Window * m_window;

	SimulationApp(real interval)
		: m_interval(interval)
		, m_paused(false)
		, m_checkingCollisions(false) {

		Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("UI.glade");

		builder->get_widget("mainWindow", m_window);
		builder->get_widget("chkCollisions", m_checkCollisions);
		builder->get_widget("btnPause", m_buttonPause);
		builder->get_widget("drawCanvas", m_drawingArea);

		m_window->set_default_size(500, 500);

		m_checkCollisions->signal_clicked().connect(
						sigc::mem_fun(*this, &SimulationApp::onCollisionsCheck));
		m_buttonPause->signal_clicked().connect(
				sigc::mem_fun(*this, &SimulationApp::onPauseButton));


		std::cout << "Initializing leapfrog simulation engine..." << std::endl;
		m_simulator = apoapsys::createBasicLeapFrogSimulator(
		NBODY_ALLOW_MAJOR_BODIES, NBODY_ALLOW_MOONS,
		NBODY_ALLOW_MINOR_BODIES);

	};
	virtual ~SimulationApp() {
	}


	void start() {
		std::vector<apoapsys::Collision *> * collisions = new std::vector<apoapsys::Collision *>();

		ulong i = 0;
		while(true) {
			if (!this->m_paused) {
				i++;
				if (i % (86400) == 0) {
					cout << "tick #" << (i / 86400) << endl;
				}
				m_simulator->step(m_interval);

				if (m_checkingCollisions) {
					m_simulator->runCollisionDetection(collisions);
					for (uint c = 0; c < collisions->size(); c++) {
						apoapsys::Collision * collision = collisions->at(c);
						std::cout << "Body '" << collision->particle0->name << "' collided with body '" << collision->particle1->name << "' on iteration #" << i << std::endl;
					}

					cleanCollisions(collisions);
					PTRDELETE(collisions);
					collisions = new std::vector<apoapsys::Collision *>();
				}
			}
		}
		std::cout << "Simulation Complete" << std::endl;
	}

};


void *exec_sim(void * _window) {
	SimulationApp * window = (SimulationApp*) _window;
	std::cout << "Executing simulation" << std::endl;
	window->start();
}

int main(int argc, char *argv[]) {
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv,
			"org.gtkmm.examples.base");



	SimulationApp window(NBODY_ITERATION_INTERVAL);
	//int result = 0;

	pthread_t simThread;
	int rc = pthread_create(&simThread, NULL, exec_sim, (void *) &app);

	return app->run(*(window.m_window));
}

#endif
