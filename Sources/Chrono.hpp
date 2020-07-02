/// \file Chrono.hpp
#ifndef CHRONO_HPP
#define CHRONO_HPP

#include <chrono>

using namespace std::chrono;

/// \brief Classe pour la gestion du temps
///
/// Classe contenant le temps écoulé afin de gérer le temps.
///
class Chrono{
private:
    duration<double> deltaTime; ///< Temps lors du dernier reset.
    steady_clock::time_point start; ///< Temps de départ
    steady_clock::time_point end; ///< Temps de fin

public:
	Chrono() {
        start = steady_clock::now();
    }

    /// \brief Remet le chrono à 0.
	void Reset(){
		start = steady_clock::now();
	}

    /// \brief Obtention du temps écoulé depuis le dernier reset.
    /// \return Temps écoulé.
	double GetTimeComplete() {
	    end = steady_clock::now();
        deltaTime = end - start;
        return deltaTime.count();
    }

    /// \brief Vérifie si un certain temps s'est écoulé depuis le dernier reset.
    /// \param delay Temps à vérifier.
    /// \return Si le temps s'est écoulé ( true ) ou pas ( false ).
	bool InDelay(double delay){
		if (GetTimeComplete() >= delay)
		    return true;
		else
		    return false;
	}
};

struct DeltaTime {
     static double time; ///< Temps écoulé depuis la dernière frame.
};
double DeltaTime::time = 0;

#endif // CHRONO_HPP
