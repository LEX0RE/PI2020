/// \file Loading.hpp
#ifndef LOADING_HPP
#define LOADING_HPP
#include "../Scene.hpp"
#include "../Event.hpp"
#include "../GLContext.hpp"
#include "../VisualComp.hpp"
#include "../SceneManager.hpp"
#include "../VisualComponents/Image.hpp"
#include "../VisualComponents/Label.hpp"
#include "../VisualComponents/LoadingBar.hpp"
#include "../ResourceManager.hpp"
#include "../Resources/Font.hpp"
#include "../Resources/Texture.hpp"
#include "../Meshes/ColoredMesh.hpp"
#include "../Meshes/TexturedMesh.hpp"
#include "../Items/ItemDerivations.hpp"
#include <map>
#include <string>
#include <queue>

using namespace std; 

/// \brief Type des resources chargées.
enum RES_TYPE {
    TEXTURE = 't',
    TMESH = 'm',
    CMESH = 'c',
    FONT = 'f'
};

/// \brief Scene de Loading
///
/// Charge les fonts, objects 3D et textures du jeu
///
class Loading : public Scene{
    LoadingBar* bar;    ///< barre de chargement
    map<string, RES_TYPE> resourceType; ///< Map contenant les types de resources.
    map<string, string> resourceFile; ///< Map contenant les noms de fichiers de resources. 
    map<uint, string> resourceName; ///< Map contenant les noms de resources.
    int resourceCount; ///< Nombre de resources.
    int resourceIndex; ///< Nombre d'itération.
    Texture* background; ///< Texture de fond.

    /// \brief Lis un fichier contenant les informations sur les resources utilisées.
    /// \param filename Nom du fichier.
    /// \return retourne vrai si le script s'ouvre, sinon faux
    bool ReadScript(const char * filename) {
        resourceType = map<string, RES_TYPE>();
        resourceFile = map<string, string>();
        resourceName = map<uint, string>();

        // ouvre le script
        if(FILE* file = fopen(filename, "r")) {
            resourceCount = 0;
            fscanf(file, "%i", &resourceCount);

            // créé la mémoire temporaire
            char* buffer = new char[2];

            for(int i = 0; i < resourceCount; i++) {
                // lecture du type
                fscanf(file, "%s", buffer);

                //on cherche les guillemets ouvrant. 
                char currentChar = fgetc(file);              
                while(currentChar != '\"') 
                    currentChar = fgetc(file);
                currentChar = fgetc(file);

                string strFileName = string();
                strFileName += currentChar;
                
                //on lit le contenu jusqu'aux guillemets fermant.
                while(currentChar != '"') {
                    currentChar = fgetc(file);
                    strFileName += currentChar;                    
                }
                strFileName.pop_back();
                currentChar = fgetc(file); //ne stokera pas '\"'.

                // si le type est une police d'écriture, alors la taille sera mis avec le nom de fichier.
                if(*buffer == 'f') {
                    strFileName += ':'; 
                    int fontSize = 0;
                    fscanf(file, "%i", &fontSize);
                    string number = to_string(fontSize);
                    strFileName += number;
                }

                // si le type est une textured mesh, alors le nom de la texture sera stocké avec le nom de fichier.
                else if(*buffer == 'm') {
                    strFileName += ':';
                    char * frameCount = new char[3];
                    fscanf(file, "%s", frameCount);
                    strFileName += frameCount;
                    delete[] frameCount;

                    strFileName += ':';
                    char * textureName = new char[30];
                    fscanf(file, "%s", textureName);
                    strFileName += textureName;
                    delete[] textureName;
                }

                // lecture et stokage du nom de la resource.
                char * resNameBuf = new char[30];
                fscanf(file, "%s", resNameBuf);
                resourceName[i] = resNameBuf;
                delete[] resNameBuf;

                // association du nom de resource à son nom de fichier.
                resourceFile[resourceName[i]] = string(strFileName);

                // association du nom de resource à son nom de fichier.
                resourceType[resourceName[i]] = (RES_TYPE)*buffer;
            }
            
            delete[] buffer;
            resourceIndex = 0;
	    fclose(file);
            return true;
        }
        return false;
    }

public:
	
	/// \brief Renvoie un pointeur de scène Loading.
    /// \param param Paramètre de transit.
	/// \return Nouvelle Instance de Loading.
    static inline Scene* LoadLoading(void* param) {
         return new Loading(); 
    }

    Loading() : Scene() {
        this->ReadScript("Resources.txt");
        this->bar = new LoadingBar({15,712}, {995, 40}, resourceCount);
        background = new Texture("resources/loadingScreen.png");
        AddOrtho(2, 
            new Image({0, 0}, {1024, 768}, background->GetId()),
            this->bar
        );

		GLContext::SetOrthogonal();
    	GLContext::ApplyOrthogonal();

        glDisable(GL_DEPTH_TEST);
    }

    ~Loading() {
        delete bar;
        delete background;

        glEnable(GL_DEPTH_TEST);
    }

    /// \brief Affiche tout les VisualComp de la scène dans la fenêtre.
    void Draw() {
        // Chargement de resources.
        string currentName = resourceName[resourceIndex];
        bar->SetText(currentName);
        switch(resourceType[currentName]) {
            case RES_TYPE::TEXTURE: {
                const char * filename = resourceFile[currentName].c_str();
                ResourceManager::Add(currentName.c_str(), new Texture(filename));
                break;
        	   }

            case RES_TYPE::FONT: {
                std::map<uint, char> size = std::map<uint, char>();
                std::map<uint, char> resourceFilename = std::map<uint, char>();
                const char * resNameArray = resourceFile[currentName].c_str();
                int it = 0;
                while(resNameArray[it] != ':') {
                    char c = resNameArray[it];
                    resourceFilename[it++] = c;
                }
                resourceFilename[it] = 0;
                int sublen = it + 1;
                while(resNameArray[it] != '\0') {
                    char c = resNameArray[it];
                    size[it - sublen] = c;
                    it++;
                }

                uint rs = resourceFilename.size();
                uint ss = resourceName.size();
                char * rarr = new char[resourceFilename.size() + 1];
                char * sarr = new char[resourceName.size() + 1];
                rarr[rs] = 0;
                sarr[ss] = 0;

                for(uint i = 0; i < rs; i++) 
                    rarr[i] = resourceFilename[i];
                for(uint i = 0; i < ss; i++) 
                    sarr[i] = size[i];

                uint sz = atoi(sarr);
                Font* font = new Font(rarr, sz);
                ResourceManager::Add(currentName.c_str(), font);
 
                break;
            }

            case RES_TYPE::TMESH: {
                std::map<uint, char> resourceName = std::map<uint, char>();
                std::map<uint, char> resourceFilename = std::map<uint, char>();
                std::map<uint, char> frameCount = std::map<uint, char>();
                const char * resNameArray = resourceFile[currentName].c_str();
            
                // itération caractères par caractères du ficher de resource.
                int it = 0;
                while(resNameArray[it] != ':') {
                    char c = resNameArray[it];
                    resourceFilename[it++] = c;
                }
                resourceFilename[it] = 0;

                // itération caractères par caractères afin d'avoir le nombre de frames.
                int sublen = it + 1;
                it++;
                while(resNameArray[it] != ':') {
                    char c = resNameArray[it];
                    frameCount[it - sublen] = c;
                    it++;
                }
                frameCount[it] = 0;

                // itération caractères par caractères pour avoir le nom de la texture spécifiée.
                sublen = it + 1;
                it++;
                while(resNameArray[it] != '\0') {
                    char c = resNameArray[it];
                    resourceName[it - sublen] = c;
                    it++;
                }
                resourceName[it] = 0;

                uint fs = frameCount.size();
                uint rs = resourceFilename.size();
                uint ns = resourceName.size();
                char * farr = new char[fs]; // nombre de frames.
                char * rarr = new char[rs]; // nom de fichier.
                char * narr = new char[ns]; // nom de texture.

                // copie.
                for(uint i = 0; i < fs; i++) 
                    farr[i] = frameCount[i];
                for(uint i = 0; i < rs; i++) 
                    rarr[i] = resourceFilename[i];
                for(uint i = 0; i < ns; i++) 
                    narr[i] = resourceName[i];

                ushort frameNumber = atoi(farr); // nombre de frame.

                TexturedMesh3* tmesh = new TexturedMesh3(frameNumber); // réglage du nombre de frames.
                Texture* t = GetResource(narr, Texture); // obtention de la texture.
                tmesh->LoadMesh((uintptr_t)t->GetInstance(), rarr); // chargement de la mesh.
                ResourceManager::Add(currentName.c_str(), tmesh); // ajout de la mesh au resource manager.

                delete[] farr;
                delete[] rarr;
                delete[] narr;
                break;
            }
        }

        resourceIndex++;

        for(auto it : ortho)
            it.second->Draw();
        bar->Load();

        if (bar->IsLoaded())
            this->NotifySceneManager(FREE, "MainMenu");
    }

};

#endif
