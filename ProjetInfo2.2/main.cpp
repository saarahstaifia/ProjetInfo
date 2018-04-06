#include "grman/grman.h"
#include "grman/widget.h"
#include <iostream>

#include "graph.h"






int main()
{
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// Un exemple de graphe
    Graph g;

bool choice= false;
bool exit=false;
int currentgraph=0;
int compteurAnim=0;

while(!key[KEY_N])
{
while(!choice && !exit)
{
    if (key[KEY_N]) exit = true;
     int check = g.Menu();

     if (check==1)
     {
         choice = true;
         g.loadFile("fichiergraphe1");
         currentgraph = 1;
     }
     if (check==2)
     {
         choice = true;
         g.loadFile("fichiergraphe2");
         currentgraph=2;
     }
     if (check==3)
     {
         choice = true;
         g.loadFile("fichiergraphe3");
         currentgraph=3;
     }
}



    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while (!key[KEY_ESC] && !exit)
    {
        compteurAnim+=1;
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();



        g.animations(&currentgraph);

        if (compteurAnim==20)
        {
            g.DynamiqueEvolution();
        }



        if(compteurAnim>50)compteurAnim=0;


    }
    g.reset();
    choice = false;
}


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


