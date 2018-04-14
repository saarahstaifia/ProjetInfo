#include "grman/grman.h"
#include "grman/widget.h"
#include <iostream>

#include "graph.h"
#include "allegro.h"






int main()
{

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");


    ///BITMAPS Du Menu
    BITMAP* bmp;
    BITMAP* menu;
    BITMAP* cadre;
    BITMAP* cadre2;
    BITMAP* regle;
    BITMAP* cadre3;

    ///declarations animaux additionnels
    BITMAP* pingouin;
    BITMAP* beluga;
    BITMAP* husky;
    BITMAP* orque;
    BITMAP* elephant;
    BITMAP* girafe;
    BITMAP* zebre;
    BITMAP* hyene;
    BITMAP* moufette;
    BITMAP* herisson;
    BITMAP* ecureuil;
    BITMAP* sanglier;


///Chargement du menu
    bmp=create_bitmap(1024,578);
    menu=create_bitmap(1024,578);
    cadre=create_bitmap(195,45);
    cadre2=create_bitmap(195,45);
    regle=create_bitmap(1024,578);
    cadre3=create_bitmap(45,35);

    ///animaux additionels
    pingouin=create_bitmap(100,100);
    beluga=create_bitmap(100,100);
    husky=create_bitmap(100,100);
    orque=create_bitmap(100,100);
    elephant=create_bitmap(100,100);
    girafe=create_bitmap(100,100);
    zebre=create_bitmap(100,100);
    hyene=create_bitmap(100,100);
    moufette=create_bitmap(100,100);
    herisson=create_bitmap(100,100);
    ecureuil=create_bitmap(100,100);
    sanglier=create_bitmap(100,100);

    menu = load_bitmap("pics/imagemenu.bmp",NULL);
    cadre=load_bitmap("pics/cadre.bmp",NULL);
    cadre2=load_bitmap("pics/cadre2.bmp",NULL);
    regle=load_bitmap("pics/regle.bmp",NULL);
    cadre3=load_bitmap("pics/cadre3.bmp",NULL);

    ///animaux additionnels
    pingouin=load_bitmap("pics/pingouin.bmp",NULL);
    beluga=load_bitmap("pics/beluga.bmp",NULL);
    husky=load_bitmap("pics/husky.bmp",NULL);
    orque=load_bitmap("pics/orque.bmp",NULL);
    elephant=load_bitmap("pics/elephant.bmp",NULL);
    girafe=load_bitmap("pics/girafe.bmp",NULL);
    zebre=load_bitmap("pics/zebre.bmp",NULL);
    hyene=load_bitmap("pics/hyene.bmp",NULL);
    moufette=load_bitmap("pics/moufette.bmp",NULL);
    herisson=load_bitmap("pics/herisson.bmp",NULL);
    ecureuil=load_bitmap("pics/ecureuil.bmp",NULL);
    sanglier=load_bitmap("pics/sanglier.bmp",NULL);



    Graph g;
///Variables gérant les allers retours avec la boucle de jeu

bool choice= false;
bool exit=false;
int currentgraph=0;
int compteurAnim=0;

while(!exit)
{
while(!choice && !exit)
{

    clear_bitmap(bmp);
///return 1/2 ou 3 selon le choix de l'utilisateur
     int check = g.Menu(bmp,menu,cadre,cadre2,regle,cadre3);


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
         currentgraph = 2;
     }
     if (check==3)
     {
         choice = true;
         g.loadFile("fichiergraphe3");
         currentgraph = 3;
     }
     if (check==5) exit=true;
}


bool go_out = false;
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while (!go_out && !exit)
    {
        compteurAnim+=1;
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
        go_out = g.Exit();

        ///Appel avec les animaux correspondants à chaque graphe

        if (currentgraph==1) g.animations(&currentgraph,bmp,pingouin,beluga,husky,orque);
        if (currentgraph==2) g.animations(&currentgraph,bmp,elephant,girafe,zebre,hyene);
        if (currentgraph==3) g.animations(&currentgraph,bmp,moufette,herisson,ecureuil,sanglier);



///toutes les secondes on appelle le ss-prog
        if (compteurAnim==20)
        {
            g.DynamiqueEvolution();
        }
///POUR SOUTENANCE --> accélérer la dynamique d'evolution
        if(!key[KEY_S])
        {
        if(compteurAnim>50)compteurAnim=0;
        }
        else
        {if(compteurAnim>25)compteurAnim=0;

        }


    }

    g.reset();
   // std::cout<<"1";
        clear_bitmap(bmp);

    // clear_bitmap(screen);

    choice = false;
}


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


