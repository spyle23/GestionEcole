/*
    ANDRIATIANA Jean-Marie
    sampingyen.lee@gmail.com
    L3 MISA 
    Gestion Ecole
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <list>
using namespace std;

//définition de la structure student
struct student{
    //attribute members
    string nom;
    vector<float> notes;

    //method members
    float moyenne();
    void display();
};

bool isNumber(const string& str); //prototype de la fonction isNumber



//définition de la structure school
struct school{
    //attribute members
    list <student> eleves;
    int nbStuds;
    vector <string> topics;
    int nbTopics;
    bool ready4data;
    string dataFile;

    //method members
    void initData();
    void displayResult();
    void addStudent();
    void remStudent();
    void saveData();
    void readData();
};

//définition de la structure menu
struct menu{
    //attribute
    vector <string> options;
    int choice;
    bool dataRead;

    //method 
    void initMenu();
    void getChoice();
    void manageChoice(school& ecole, bool &done);
};

int main(){
    cout << "\t\t\t gestion de notes" <<endl;
    bool done(false);
    menu myMenu;
    school ecole;
    //on initialise les données
    ecole.initData();

    //on initialise le menu
    myMenu.initMenu();


    //Gère les tâches avec le menu
    
    while(!done){
        myMenu.getChoice();
        myMenu.manageChoice(ecole, done);
    }
    return 0;
}
//la fonction isNumber
bool isNumber(const string& str){
    for(char const &c : str){
        if(std::isdigit(c)==0) return false;
    }
    return true;
}

/* Menu */

//la méthode initMenu
void menu::initMenu(){
    dataRead = false;
    choice = -1; 
}

//la méthode getChoice
void menu::getChoice(){
    //on affiche le menu 
    cout << "tapez: " << endl;
    options.push_back("1 pour Afficher les resultats");
    options.push_back("2 pour Lire les donnees sur disque");
    options.push_back("3 pour Ajouter un eleve");
    options.push_back("4 pour Sauver les donnees");
    options.push_back("5 pour retirer un eleve");
    options.push_back("0 pour Quitter ce programme");
    for (int i = 0; i < 6; i++)
    {
        cout << "\t\t" << options[i] << endl; 
    }
    cout << "votre choix: ";
    cin >>choice;

    if(choice<0 || choice>4){
        getChoice();
    }    
}
//la méthode manageChoice
void menu::manageChoice(school& ecole, bool &done){
        switch (choice)
        {
        case 0:
            done = true;
            break;
        case 1:
            if(ecole.ready4data==true || dataRead==true){
                ecole.displayResult();
            }   
            break;
        case 2:
            ecole.readData();
            dataRead = true; //donnée lue
            break;
        case 3:
            ecole.addStudent();
            break;
        case 4:
            ecole.saveData();
            break;
        case 5:
            ecole.remStudent();
            break;
        }
}


/* Student */

//définition de la méthode moyenne
float student::moyenne(){
    int nbNotes = notes.size();
    float somme(0);
    for(int i=0; i<nbNotes; i++){
        somme += notes[i];
    }
    return (somme/notes.size());
}

//définition de la méthode display
void student::display(){
    int nbNotes = notes.size();
    for(int i=0; i<nbNotes; i++){
        cout <<"\t\t"<< notes[i];
    }
    cout << endl;

    cout << "la moyenne est: "<< moyenne() << endl;
}

/* School */

//définition de la méthode displayResult
void school::displayResult(){
        list <student>::iterator it;    //on utilise un itérateur pour parcourir la liste
        for (int i=0; i<nbTopics ; i++)
        {
            cout << "\t\t" << topics[i];   
        }
        cout << endl;
        for (auto it=eleves.begin(); it != eleves.end(); ++it)
        {
            cout << it -> nom << endl;  
            it -> display();
        }
}

//définition de la méthode initData
void school::initData(){
    nbTopics = 3;
    nbStuds = 0;
    topics.push_back("Math");
    topics.push_back("Info");
    topics.push_back("Comm");
    dataFile = "data.txt";
}

//définition de la méthode addStudent
void school::addStudent(){
    student temporaire;
    cout <<"entrer le nom du nouveau eleve: ";
    cin >> temporaire.nom;

    for (int i = 0; i < nbTopics; i++)
    {
        float note(0);
        cout << "entrer la note: ";
        cin >> note;

        temporaire.notes.push_back(note);
    }
    eleves.push_back(temporaire);
    nbStuds++;
    ready4data = true;    
}

//définition de la méthode remStudent
void school::remStudent(){
    if (eleves.empty())
    {
        cout << "rien a effacer" << endl;
        return ;
    }
    eleves.pop_front();
    
    cout << "l'eleve a bien ete supprime" << endl;
    
}

//définition de la méthode saveData
void school::saveData(){
    //création du flux de sortie
    ofstream outFile;
    outFile.open(dataFile);

    if (outFile.is_open()) {
        //on enregistre le nombre de UE et d'etudiants sur le fichier
        outFile << nbTopics << endl;
        outFile << nbStuds << endl;
        for (int i = 0; i < nbTopics; i++)
        {
            outFile << topics[i] << " ";
        }
        outFile << endl;
        for (auto& itst: eleves) {
            outFile << itst.nom << ' ';
            outFile << endl;
            for (auto& it: itst.notes){
                outFile << it << ' ';
                outFile << endl;   
            }
        }

        cout << "Données enregistrés avec succès!\n";
    
    // traitement terminé
        outFile.close();
    }
    else cout << "pb de sauvegarde " << endl;
   
    
}

//définition de la méthode readData
void school::readData(){
    ifstream inFile;    // création du flux d'entrée
    string tstr;
    float tnum;
    int nbNewStuds(0);

    if (ready4data==false) initData();
    inFile.open(dataFile);

    if (inFile.is_open()) {
        inFile >> nbTopics ;
        inFile >> nbNewStuds;
        
        for (int i = 0; i < nbTopics; i++)
        {
            inFile >> tstr;
            topics.push_back(tstr);
        }
        for (int i=0; i<nbNewStuds; ++i) {
         // Un nouveau étudiant
            student eleve;
            inFile >> eleve.nom;
            for (int j=0; j<nbTopics; ++j) {
                float note(0.0f);
                inFile >> note;
                eleve.notes.push_back(note);
            }
            eleves.push_back(eleve);
        }
        cout << "Donnees lus avec succes!\n";

    // traitement terminé
        inFile.close();
    } else
        cerr << "Le fichier '" << dataFile << "' est introuvable!";
}


