/// \file SceneManager.hpp
#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include "Scene.hpp"
#include "GLContext.hpp"
#include "Event.hpp"
#include "Chrono.hpp"
#include "VisualComponents/Label.hpp"
#include <stack>
/// \brief Classe gérant les scènes.
///
/// Classe qui gère les scènes comme celle qui est active ou la prochaine scène à afficher.
///
class SceneManager {
	static std::map<std::string, Scene*(*)(void*)> staticCtor; ///< Membre statique liant un nom de scène à un pointeur de fonction afin de pouvoir charger un scène n'importe quand.
	static std::list<Scene*> sceneStack; ///< liste (utilisé comme une pile) de scène.
	static std::list<string> nameStack; ///< liste (utilisé comme une pile) de nom de scène.
	static std::string currentSceneName; ///< Nom de la scène active.
	static Scene* currentScene; ///< Pointeur sur la scène active.
	static void* transit; ///< Paramètre de transit.
	template <class T> using ListIt = typename std::list<T>::iterator; ///< Typedef pour rendre la déclaration d'itérateur moins longue

public:
    static Chrono* frameChrono; ///< Chrono pour l'update du frameLabel et du dTLabel.
    static uint frameCount; ///< Compte du nombre de frame.
    static Label* frameLabel; ///< Affichage du fps.
    static Label* dTLabel; ///< Affichage du deltaTemps.
	static bool isOpen; ///< Indique si la fenêtre est ouvert ou fermée.
	
	/// \brief Ajoute un pointeur de fonction ainsi que du nom de la scène chargée.
	/// \param name Nom de la scène.
	/// \param staticConstructor Fonction statique appelé par le gestionnaire lors du changement de scène.
	static inline void AddScene(std::string name, Scene*(*staticConstructor)(void*)) {
		staticCtor[name] = staticConstructor;
	}

	/// \brief règle la scène active.
	/// \param scene Nom de la scène active.
	static inline void SetCurrentScene(std::string scene) {
		currentScene = staticCtor[scene](transit);
		currentSceneName = scene;
		sceneStack.push_front(currentScene);
		nameStack.push_front(scene);
	}
	
	/// \brief Méthode pour recevoir le nom de la scène active.
	/// \return le nom de la scène active.
	static inline std::string GetCurrentSceneName() {
		return currentSceneName;
	}

	/// \brief Permets de modifier une scène alors qu'elle est empilée.
	/// \param name Nom de la scène à modifier.
	/// \return Retourne une scène. NE PAS SUPPRIMER. Si l'opération a échoué, nullptr sera retourné.
	static Scene* GetStackedScene(std::string name) {
		ListIt<std::string> itString = nameStack.begin();
		ListIt<std::string> itStrEnd = nameStack.end();
		ListIt<Scene*> itScene = sceneStack.begin();
		while(itString != itStrEnd) {
			if(*(itString) == name)
				return *itScene;
			itScene++;
			itString++;
		}
		return nullptr;
	}
	
	/// \brief règle la scène active.
	/// \return le pointeur de la scène active.
	template <class T> static inline T* GetCurrentScene() {
		return (T*)currentScene;
	}
	
	/// \brief affiche la scène active.
	static void Draw() {
		Chrono mathChrono = Chrono();

		while(isOpen){
			while(Event::Poll()) {
				switch(Event::GetType()){
					case SDL_QUIT: {
						isOpen = false;
					}
					break;
					default: {
						currentScene->Notify(Event::GetType()); // Notifie la scène qu'il y a eu un évenement.
					}
				}
			}

            DeltaTime::time = mathChrono.GetTimeComplete();
            mathChrono.Reset();
            GLContext::Clear(); //NE PAS ENLEVER. -Gaël

            if(currentScene)

			{
				switch(currentScene->options) {
					case FREE: { // libère la scène et la remplace.
						string nextScene = currentScene->nextScene;
						if(staticCtor[nextScene]) { //vérifie si le constructeur statique de la prochaine scène existe.
							transit = currentScene->transit; // Ajouté.
							delete currentScene;
							sceneStack.pop_front();
							nameStack.pop_front();
							currentScene = staticCtor[nextScene](transit);
							currentSceneName = nextScene;
							sceneStack.push_front(currentScene);
							nameStack.push_front(currentSceneName);
						}
						currentScene->options = DRAW; // Pour éviter les boucles infinies... Sert aussi d'indicateur qu'il y a eu une erreur. 	
						break;
					}

					case DRAW: { //dessine la scène.
						currentScene->Draw();
                        frameCount++;
                        if(frameChrono->GetTimeComplete() > 1){
                            string s = "FPS: ";
                            frameLabel->SetText(s.append(to_string(frameCount)).c_str());
                            dTLabel->SetText(("DeltaTemps: " + to_string(DeltaTime::time)).c_str());
                            frameChrono->Reset();
                            frameCount = 0;
                        }
                        frameLabel->Draw();
                        dTLabel->Draw();
						break;
					}

					case PUSH: { // empile une scène temporairement.
						string nextScene = currentScene->nextScene;
						if(staticCtor[nextScene]) {
							transit = currentScene->transit; // Ajouté.
							currentScene = staticCtor[nextScene](transit);
							currentSceneName = nextScene;
							nameStack.push_front(nextScene);
							sceneStack.push_front(currentScene);
						}
						currentScene->options = DRAW;
						break;
					}

					case POP: { // dépile une scène.
						if(sceneStack.size()) {
							delete currentScene;
							sceneStack.pop_front();
							nameStack.pop_front();
							currentScene = *sceneStack.begin();
							currentSceneName = *nameStack.begin();
						}
						currentScene->options = DRAW;
						break;
					}
				}
			}
			GLContext::Refresh(); //NE PAS ENLEVER -Gaël
		}
	}

};
std::map<std::string, Scene*(*)(void*)> SceneManager::staticCtor = std::map<std::string, Scene*(*)(void*)>();
Scene* SceneManager::currentScene = nullptr;
std::string SceneManager::currentSceneName = "null";
std::list<Scene*> SceneManager::sceneStack = std::list<Scene*>();
std::list<string> SceneManager::nameStack = std::list<string>();
void* SceneManager::transit = nullptr;
Chrono* SceneManager::frameChrono = new Chrono();
uint SceneManager::frameCount = 0;
Label* SceneManager::frameLabel = nullptr;
Label* SceneManager::dTLabel = nullptr;
bool SceneManager::isOpen = true;
#endif
