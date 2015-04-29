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
private:
         Glib::RefPtr<Gtk::Builder> refBuilder;
protected:
	std::vector<apoapsys::Particle *> * m_particles;

	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
		//cout << "Drawing..." << endl;
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		// coordinates for the center of the window
		int xc, yc;
		xc = width / 2;
		yc = height / 2;

		return true;
	}

	void forceRedraw() {
		Glib::RefPtr<Gdk::Window> win = get_window();
		if (win) {
			Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
			win->invalidate_rect(r, false);
		}
	}

	bool onRedrawTimeout() {
	//	this->forceRedraw();
		return true;
	}

public:
	SimulationDisplay(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder)
		: Gtk::DrawingArea(cobject)
		, refBuilder(builder){
		signal_draw().connect(sigc::mem_fun(*this, &SimulationDisplay::on_draw), false);
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &SimulationDisplay::onRedrawTimeout), 100 );
	}
	virtual ~SimulationDisplay() {

	}



	void setParticles(std::vector<apoapsys::Particle *> * particles) {
		this->m_particles = particles;
	}

};


class SimulationState {

public:
	bool paused;
	real interval;
	bool checkingCollisions;

	SimulationState()
		: paused(false)
		, interval(1.0)
		, checkingCollisions(false) {

	}
};

/* Poorly designed and constructed window
 *
 */
class SimulationApp {
private:

protected:




	apoapsys::LeapFrogSimulator * m_simulator;
	SimulationState state;

	void onCollisionsCheck() {
		//state.checkingCollisions = m_checkCollisions->get_active();
	}

	void onPauseButton() {
		if (state.paused) {
			cout << "Unpausing..." << endl;
		//	this->m_buttonPause->set_label("Pause");
		} else {
			cout << "Pausing..." << endl;
		//	this->m_buttonPause->set_label("Unpause");
		}
		state.paused = !state.paused;
	}

	bool onTick() {
		if (state.paused) {
			return true;
		}

		cout << "*tick*" << endl;
		for (uint i = 0; i < 86400; i++) {
			m_simulator->step(state.interval);
		}

		if (state.checkingCollisions) {
			std::vector<apoapsys::Collision *> * collisions = new std::vector<apoapsys::Collision *>();
			m_simulator->runCollisionDetection(collisions);
			for (uint c = 0; c < collisions->size(); c++) {
				apoapsys::Collision * collision = collisions->at(c);
				std::cout << "Body '" << collision->particle0->name << "' collided with body '" << collision->particle1->name << std::endl;
			}

			cleanCollisions(collisions);
			PTRDELETE(collisions);

		}

		return true;
	}

public:
	Gtk::Window * m_window;

	SimulationApp(real interval) {


		Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("UI.glade");

		SimulationDisplay * m_drawingArea;
		Gtk::Button * m_buttonPause;
		Gtk::CheckButton * m_checkCollisions;

		builder->get_widget("mainWindow", m_window);
		builder->get_widget("chkCollisions", m_checkCollisions);
		builder->get_widget("btnPause", m_buttonPause);

		//builder->get_widget_derived("drawCanvas",m_drawingArea);

		m_window->set_default_size(500, 500);

		m_checkCollisions->signal_clicked().connect(
						sigc::mem_fun(*this, &SimulationApp::onCollisionsCheck));
		m_buttonPause->signal_clicked().connect(
				sigc::mem_fun(*this, &SimulationApp::onPauseButton));


		std::cout << "Initializing leapfrog simulation engine..." << std::endl;
		m_simulator = apoapsys::createBasicLeapFrogSimulator(NBODY_ALLOW_MAJOR_BODIES, NBODY_ALLOW_MOONS,NBODY_ALLOW_MINOR_BODIES);

		//Glib::signal_timeout().connect( sigc::mem_fun(*this, &SimulationApp::onTick), 100 );
		//m_drawingArea->setParticles(m_simulator->getParticles());
	};
	virtual ~SimulationApp() {
	}

	void start() {
		ulong i = 0;
		while(true) {
			this->onTick();
		}

	}

};


void *exec_sim(void * _window) {
	SimulationApp * window = (SimulationApp*) _window;
	std::cout << "Executing simulation" << std::endl;
	window->start();
	std::cout << "Simulation Complete" << std::endl;
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
