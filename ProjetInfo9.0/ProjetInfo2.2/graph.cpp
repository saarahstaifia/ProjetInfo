#include "graph.h"
#include <fstream>
#include <sstream>
#include "grman/widget.h"
#include "allegro.h"
#include "time.h"


/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_border(3);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(18,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );

    m_box_label_idx.add_child( m_label_grp );
    m_label_grp.set_posy(-70);




    ///TECHNIQUE POUR CONVERTIR SANS LA FONCTION RELOU
    std::ostringstream strs;
    strs << idx;
    std::string str = strs.str();
     m_label_idx.set_message( str );
}


/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    std::ostringstream strs;
    strs << m_value;
    std::string str = strs.str();

   m_interface->m_label_value.set_message( str );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(18,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0 , 100); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(12,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider

    std::ostringstream strs;
    strs << m_weight;
    std::string str = strs.str();
    m_interface->m_label_weight.set_message( str ) ;
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();

}



/***************************************************
                    GRAPH
****************************************************/


/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);

    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
    m_main_box.add_child(m_img_fond);
    m_img_fond.set_pos(0,0);

    m_top_box.add_child(m_checkbox);
    m_checkbox.set_dim(50,40);
    m_checkbox.set_pos(15,520);
    m_checkbox.add_child(m_text5);
    m_text5.set_message("Animation");
    m_text5.set_posy(-20);

    m_top_box.add_child(m_checkbox2);
    m_checkbox2.set_dim(50,40);
    m_checkbox2.set_pos(15,450);
    m_checkbox2.add_child(m_text7);
    m_text7.set_message("Impact Humain");
    m_text7.set_posy(-20);

    ///Bouton Sauvegarde
    m_top_box.add_child(m_button1);
    m_button1.set_dim(50,40);
    m_button1.set_pos(15,10);
    m_button1.set_bg_color(BLEUCLAIR);
    m_button1.add_child(m_img_save);
    m_img_save.set_pic_name("save.jpg");


    m_top_box.add_child(m_exit);
    m_exit.set_dim(50,40);
    m_exit.set_pos(15,375);
    m_exit.add_child(m_img_save7);
    m_img_save7.set_pic_name("exit.jpg");




    //m_img_fond.set_pic_name("save.jpg");


    m_top_box.add_child(m_button2);
    m_button2.set_dim(50,40);
    m_button2.set_pos(15,60);
    m_button2.add_child(m_text1);
    m_text1.set_message("Supr Sommet");
    m_button2.add_child(m_img_save2);
    m_img_save2.set_pic_name("somme2.jpg");
    m_text1.set_posy(45);


    m_top_box.add_child(m_button3);
    m_button3.set_dim(50,40);
    m_button3.set_pos(15,240);
    m_button3.add_child(m_text2);
    m_text2.set_message("Add Arete");
    m_button3.add_child(m_img_save3);
    m_img_save3.set_pic_name("somme.jpg");
    m_text2.set_posy(45);

    m_top_box.add_child(m_button4);
    m_button4.set_dim(50,40);
    m_button4.set_pos(15,120);
    m_button4.add_child(m_text3);
    m_text3.set_message("Supr Arete");
    m_button4.add_child(m_img_save4);
    m_img_save4.set_pic_name("somme2.jpg");
    m_text3.set_posy(45);

    m_top_box.add_child(m_button5);
    m_button5.set_dim(50,40);
    m_button5.set_pos(15,180);
    m_button5.add_child(m_text4);
    m_text4.set_message("Add Sommet");
    m_button5.add_child(m_img_save5);
    m_img_save5.set_pic_name("somme.jpg");
    m_text4.set_posy(45);

    m_top_box.add_child(m_button6);
    m_button6.set_dim(50,40);
    m_button6.set_pos(15,300);
    m_button6.add_child(m_text6);
    m_text6.set_message("Fort Connex.");
    m_text6.set_posy(45);
    m_button6.add_child(m_img_save6);
    m_img_save6.set_pic_name("reseau.jpg");

}

///Ss programme pour lire un fichier et remplir un graphe
///chargement depui un fichier
void Graph::loadFile(std::string fileName)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///varialbe string pour sauter des lignes
    std::string nom;
    ///information utile pour faire le sommet
    int numS, x, y;
    double valeur;
    float Coef;
    std::string pic_name;
    if(fileName+".txt"=="fichiergraphe1.txt" && m_saved1==true) fileName="save1";
    if(fileName+".txt"=="fichiergraphe2.txt" && m_saved2==true) fileName="save2";
    if(fileName+".txt"=="fichiergraphe3.txt" && m_saved3==true) fileName="save3";
    ///lecture du fichier
    std::ifstream fichier(fileName+".txt",std::ios::in);
    if(fichier)
    {
         if(fileName+".txt"=="fichiergraphe1.txt" || fileName+".txt"=="save1.txt")  m_interface->m_img_fond.set_pic_name("banquise.jpg");
         if(fileName+".txt"=="fichiergraphe2.txt" || fileName+".txt"=="save2.txt")  m_interface->m_img_fond.set_pic_name("savane.jpg");
         if(fileName+".txt"=="fichiergraphe3.txt" || fileName+".txt"=="save3.txt")  m_interface->m_img_fond.set_pic_name("foret.jpg");
        ///on récupére le premier caractère puis on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on remplit notre vector intérmédiare en fonction de l'ordre du graphe avec des 0
        for(int i=0; i<m_ordre; i++)
        {
            fichier>>numS>>valeur>>x>>y>>Coef>>pic_name;
            add_interfaced_vertex(numS,valeur,x,y,pic_name);

    std::ostringstream strs;
    strs << Coef;
    std::string str = strs.str();
    m_vertices[numS].m_interface->m_top_box.set_Coef(Coef);

    m_vertices[numS].m_interface->m_label_idx.set_message(str);
        }
        std::cout<<numS;
        if( fileName=="save1" && m_saved1) m_ordre=numS+1;
        if( fileName=="save2" && m_saved2) m_ordre=numS+1;
        if( fileName=="save3" && m_saved3) m_ordre=numS+1;

        fichier>>m_nbArete;
        getline(fichier,nom);
        ///Adjacences
        for(int i=0; i<m_nbArete; i++)
        {
            fichier>>numS>>x>>y>>valeur;

            add_interfaced_edge(numS,x,y,valeur);
        }
        if( fileName=="save1" && m_saved1) m_nbArete=numS+1;
        if( fileName=="save2" && m_saved2) m_nbArete=numS+1;
        if( fileName=="save3" && m_saved3) m_nbArete=numS+1;

        fichier.close();
    }

    else
    {
        std::cout<<"Il n'exite pas de fichier portant ce nom";
    }
   // std::cout<<m_vertices[3].m_interface->m_top_box.get_posx();
}
///sous programme qui detecte la sortie
bool Graph::Exit()
{
    if(m_interface->m_exit.get_value()) return true;
    return false;
}

///sauvegarde et ouverture en écriture dans un fichier
void Graph::saveFile(std::string fileNameSave)
{

    // std::cout<<m_vertices[3].m_interface->m_top_box.get_posx();
    std::ofstream fichier(fileNameSave, std::ios::out);  // ouverture en écriture avec
    if (fichier)
    {
        if(fileNameSave=="save1.txt") m_saved1 = true;
        if(fileNameSave=="save2.txt") m_saved2 = true;
        if(fileNameSave=="save3.txt") m_saved3 = true;
        int cpt=0;
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {

                cpt+=1;

        }
        fichier << cpt<<"\n";
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
           ///index
           fichier << it -> first<<" " ;
           ///valeur
           fichier << it->second.m_value<<" ";
           ///posx
           fichier << m_vertices[it->first].m_interface->m_top_box.get_posx() + 2 <<" ";
           ///posy
           fichier << m_vertices[it->first].m_interface->m_top_box.get_posy() + 2<<" ";
           ///Coef Croissance
           fichier << m_vertices[it->first].m_interface->m_top_box.get_Coef() <<" ";

           ///img de la photo
           fichier << m_vertices[it->first].m_interface->m_img.get_pic_name()<<"\n";


        }
        cpt =0;
        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
                cpt+=1;
        }

        fichier << cpt<<"\n";
        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
           ///index
           fichier << it -> first<<" " ;
           ///départ
           fichier << it->second.m_from<<" ";
           ///arrivée
           fichier << it-> second.m_to<<" ";
           /// poids
           fichier << it-> second.m_weight<<"\n";


        }
        fichier.close();
    }
         else
                std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}



/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();


    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}
///fichier << m_vertices[i].m_interface->m_top_box.get_posx();

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);
    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;
    m_reverse_edges[idx] = Edge(weight, ei);
    m_reverse_edges[idx].m_from = id_vert2;
    m_reverse_edges[idx].m_to = id_vert1;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);

}
/// eidx index of edge to remove
void Graph::remove_edge(int eidx)
{
/// référence vers le Edge à enlever
Edge &remed=m_edges.at(eidx);





/// test : on a bien des éléments interfacés
if (m_interface && remed.m_interface)
{

m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
}


std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
std::vector<int> &veto = m_vertices[remed.m_to].m_in;
vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

m_edges.erase( eidx );
m_reverse_edges.erase(eidx);



//m_nbArete--;

}
///retirer un sommet
void Graph::remove_vertex(int vidx)
{
    /// référence vers le Edge à enlever
    Vertex &supr=m_vertices.at(vidx);

    ///suppresion des arretes liées à ce sommet
    for(int i = supr.m_in.size() -1 ; i >=0 ; i--)
        remove_edge(supr.m_in[i]);
    for(int i =supr.m_out.size() -1 ; i >=0  ; i--)
        remove_edge(supr.m_out[i]);

        ///on check les éléments interfacés -> go delete if oui
        if (m_interface && supr.m_interface)
        {
            m_interface->m_main_box.remove_child( supr.m_interface->m_top_box );
        }

        ///on le supprime de la map
        m_vertices.erase(vidx);
        //m_ordre--;

}
///ajouter un sommet
 void Graph::add_vertex(std::string animal)
{
    add_interfaced_vertex(m_ordre,30,mouse_x,mouse_y,animal);
    m_ordre++;


}
///ajouter une arete
void Graph::add_edge(int from, int to)
{

    add_interfaced_edge(m_nbArete,from,to,30);
    m_nbArete++;


}

///ssprog qui retourne l'index d'un sommet quand on clique dessus
int Graph::ReturnIdx()
{
    while(!mouse_b&1)
    {

    }
        int TuttoBene= -1;
            if(mouse_b&1)
            {


                for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {


            ///si la souris est comprise dans les coords de la Top box en question; on affiche le num de cette top box

               if( (( mouse_x -111 > m_vertices[it->first].m_interface->m_top_box.get_posx() ) ) && (mouse_x -111 < m_vertices[it->first].m_interface->m_top_box.get_posx() + m_vertices[it->first].m_interface->m_top_box.get_dimx() + 5 ) && (mouse_y > m_vertices[it->first].m_interface->m_top_box.get_posy() ) && (mouse_y< m_vertices[it->first].m_interface->m_top_box.get_dimy() + m_vertices[it->first].m_interface->m_top_box.get_posy() + 5  ))
               {
                  /// std::cout<< it->first<<"--"<<m_vertices[it->first].m_interface->m_top_box.get_posx()<<"--"<<m_vertices[it->first].m_interface->m_top_box.get_posy()<<"\n";
                    return (it->first);
                    TuttoBene = 0;
               }
        }
        }
if(TuttoBene == -1)
{ return -1;}
}

///partie graphique
void Graph::animations(int*currentgraph,BITMAP*bmp,BITMAP* animal1,BITMAP* animal2,BITMAP* animal3,BITMAP* animal4)
{
///Sauvegarde
    if (m_interface->m_button1.clicked())
    {
        if (*currentgraph==1) saveFile("save1.txt") ;
        if (*currentgraph==2) saveFile("save2.txt") ;
        if (*currentgraph==3) saveFile("save3.txt") ;
    }



    ///Suppresion Sommet
    if (m_interface->m_button2.clicked())
    {
    bool ok = false;

    while(mouse_b&1)
    {
    }
    while(!ok)
    {
        int Idx = ReturnIdx();
        if (Idx != -1)
        {ok = true;
        remove_vertex(Idx);}


    }
    }

///Ajout Arete
        if (m_interface->m_button3.clicked())
    {
    bool ok = false;

    while(mouse_b&1)
    {
    }
    while(!ok)
    {
        int Idx = ReturnIdx();
        rest(100);
        int Idx2 = ReturnIdx();
        if (Idx != -1 && Idx2 != -1  && Idx!=Idx2)
        {
        ok = true;
        add_edge(Idx,Idx2);

        }
        if(mouse_b&2) ok = true;

        }

    }


    ///Supression arete

    if (m_interface->m_button4.clicked())
    {
    bool ok = false;

    while(mouse_b&1)
    {
    }
    while(!ok)
    {
        int Idx = ReturnIdx();
        rest(100);
        int Idx2 = ReturnIdx();
        if (Idx != -1 && Idx2 != -1  && Idx!=Idx2)
        {

    int Idexe;
    int From;
    int To;
    int IdxAreteToDelete=-1;
   // std::ofstream fichier(fileNameToCheck, std::ios::out);  // ouverture en écriture avec

        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
           ///index
            Idexe = it -> first;
           ///départ
           From =  it->second.m_from;
           ///arrivée
           To = it-> second.m_to;

           if (From == Idx && To == Idx2)
           {

               IdxAreteToDelete = Idexe;
           }
        }
            if (IdxAreteToDelete!= -1)
             {
        // m_vertices[Idx].m_out[Idx2]
        ok = true;
        remove_edge(IdxAreteToDelete);

        }
        }
        if(mouse_b&2) ok = true;


    }
    }

    if (m_interface->m_button6.clicked())
    {   if(mouse_b&1) FindComposanteFortementConnexe();
        if (mouse_b&2)
        {
            for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            m_vertices[it->first].m_interface->m_top_box.set_bg_color(GRISCLAIR);
        }
        }
        }


///Add Vertex
    if (m_interface->m_button5.clicked())
    {
        while(mouse_b&1)
        {

        }
        while(!mouse_b&1)
        {
            clear_to_color(bmp,makecol(255,0,255));
            draw_sprite(bmp,animal1,120,100);
            draw_sprite(bmp,animal2,120,210);
            draw_sprite(bmp,animal3,120,320);
            draw_sprite(bmp,animal4,120,430);
            draw_sprite(screen,bmp,0,0);

            ///APPELLE DEPENDANT DE L'ANIMAL AJOUTé

            if(mouse_b&1)
            {
                if(*currentgraph==1)
                {
                    if(mouse_x>120 && mouse_x<220 && mouse_y>100 && mouse_y<200)
                    {
                        add_vertex("pingouin.jpg");
                    m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(34);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("34");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>210 && mouse_y<310)
                    {
                        add_vertex("beluga.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(390);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("390");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>320 && mouse_y<420)
                    {
                        add_vertex("husky.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(63);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("63");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>430 && mouse_y<530)
                    {
                        add_vertex("orque.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(480);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("480");
                    }
                }
                if(*currentgraph==2)
                {
                    if(mouse_x>120 && mouse_x<220 && mouse_y>100 && mouse_y<200)
                    {
                        add_vertex("elephant.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(660);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("660");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>210 && mouse_y<310)
                    {
                        add_vertex("girafe.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(440);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("440");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>320 && mouse_y<420)
                    {
                        add_vertex("zebre.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(375);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("375");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>430 && mouse_y<530)
                    {
                        add_vertex("hyene.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(100);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("100");
                    }
                }
                if(*currentgraph==3)
                {
                    if(mouse_x>120 && mouse_x<220 && mouse_y>100 && mouse_y<200)
                    {
                        add_vertex("moufette.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(66);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("66");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>210 && mouse_y<310)
                    {
                        add_vertex("herisson.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(37);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("37");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>320 && mouse_y<420)
                    {
                        add_vertex("ecureuil.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(38);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("38");
                    }
                    if(mouse_x>120 && mouse_x<220 && mouse_y>430 && mouse_y<530)
                    {
                        add_vertex("sanglier.jpg");
                        m_vertices[m_ordre-1].m_interface->m_top_box.set_Coef(115);
                    m_vertices[m_ordre-1].m_interface->m_label_idx.set_message("115");
                    }
                }
            }

        }
        clear_to_color(bmp,makecol(255,0,255));
            draw_sprite(screen,bmp,0,0);

    }
    }
///Menu dependant des choix utilisateurs
    int Graph::Menu(BITMAP*bmp,BITMAP*menu,BITMAP*cadre,BITMAP* cadre2, BITMAP*regle,BITMAP*cadre3)
    {
        draw_sprite(bmp,menu,0,0);
        if(mouse_x>78 && mouse_x<78+190 && mouse_y>155 && mouse_y<195 && mouse_b&1)
        {
            draw_sprite(bmp,cadre2,78,155);
            draw_sprite(screen,bmp,0,0);
            rest(100);
            clear_bitmap(bmp);
             return(1);
        }
        else{
            if(mouse_x>78 && mouse_x<78+190 && mouse_y>155 && mouse_y<195) draw_sprite(bmp,cadre,78,155);
        }
        if(mouse_x>78 && mouse_x<78+190 && mouse_y>237 && mouse_y<277 && mouse_b&1)
        {
            draw_sprite(bmp,cadre2,78,237);
            draw_sprite(screen,bmp,0,0);
            rest(100);
            clear_bitmap(bmp);
            return (2);
        }
        else{
            if(mouse_x>78 && mouse_x<78+190 && mouse_y>237 && mouse_y<277) draw_sprite(bmp,cadre,78,237);
        }
        if(mouse_x>78 && mouse_x<78+190 && mouse_y>317 && mouse_y<357 && mouse_b&1)
        {
            draw_sprite(bmp,cadre2,78,317);
            draw_sprite(screen,bmp,0,0);
            rest(100);
            clear_bitmap(bmp);
            return(3);
        }
        else{
            if(mouse_x>78 && mouse_x<78+190 && mouse_y>317 && mouse_y<357) draw_sprite(bmp,cadre,78,317);
        }
        if(mouse_x>78 && mouse_x<78+190 && mouse_y>395 && mouse_y<435 && mouse_b&1)
        {
           draw_sprite(bmp,cadre2,78,395);
           draw_sprite(screen,bmp,0,0);
           rest(100);


            while(!(mouse_x>37 && mouse_x<82 && mouse_y>524 && mouse_y<557 && mouse_b&1))
            {

            clear_bitmap(bmp);
            draw_sprite(bmp,regle,0,0);
            if(mouse_x>37 && mouse_x<82 && mouse_y>524 && mouse_y<557) draw_sprite(bmp,cadre3,38,524);
            draw_sprite(screen,bmp,0,0);
            }

        }
        else{
            if(mouse_x>78 && mouse_x<78+190 && mouse_y>395 && mouse_y<435) draw_sprite(bmp,cadre,78,395);
        }
        if(mouse_x>78 && mouse_x<78+190 && mouse_y>472 && mouse_y<512 && mouse_b&1)
        {
            draw_sprite(bmp,cadre2,78,472);
            draw_sprite(screen,bmp,0,0);
            rest(100);
            clear_bitmap(bmp);
            return (5);
        }
        else{
            if(mouse_x>78 && mouse_x<78+190 && mouse_y>472 && mouse_y<512) draw_sprite(bmp,cadre,78,472);
        }



        draw_sprite(screen,bmp,0,0);

        clear_bitmap(bmp);
    }
///vider tous les vecteurs
    void Graph::reset()
    {

        m_vertices.clear();
        m_edges.clear();
        m_reverse_edges.clear();



    }
    ///animation des populations
    void Graph::DynamiqueEvolution()
    {
        if(m_interface->m_checkbox.get_value())
        {
            for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            m_vertices[it->first].m_interface->m_top_box.set_moveable(false);
        }



        float K;
        float soustraction;

         for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {

            K = 0;
            soustraction = 0;

        ///on établit le calcul de la population à T+1 depuis T
    Vertex &supr=m_vertices.at(it->first);

    ///check les aretes qui arrivent et qui partent pour trouver le K et quoi soustraire
    for(int i = supr.m_in.size() -1 ; i >=0 ; i--)
    {


        K += ((m_edges[supr.m_in[i]].m_weight)*0.06) * m_vertices[m_edges[supr.m_in[i]].m_from].m_interface->m_slider_value.get_value();


       ///--> la partie négative de l'équation de croissance += le poids de l'arete entrante /2500 (coef approximatif pour temporiser la croissance) * la pop du "mangeur"
    }

    if(m_vertices[it->first].m_interface->m_top_box.get_Coef()==1)
    {if(m_interface->m_checkbox2.get_value())
    {
        K=40;
    }
    else
    {
       K=70;
    }
    }
        for(int i =supr.m_out.size() -1 ; i >=0  ; i--)
      {
          soustraction += ((m_edges[supr.m_out[i]].m_weight)*0.0006) * m_vertices[m_edges[supr.m_out[i]].m_from].m_interface->m_slider_value.get_value();

      }


      /// ON APPLIQUE LA FORMULE
      float Set_Value = (m_vertices[it->first].m_interface->m_slider_value.get_value() + ((1/((m_vertices[it->first].m_interface->m_top_box.get_Coef())))*(m_vertices[it->first].m_interface->m_slider_value.get_value()))*(1-(m_vertices[it->first].m_interface->m_slider_value.get_value())/(K)) - soustraction);
      ///ON MET A JOUR L'AFFICHAGE
      if (soustraction == 0) Set_Value = 1.02*Set_Value;



      int test = Set_Value;
      //m_vertices[it->first].m_interface->m_slider_value.set_value(Set_Value);

            if (m_vertices[it->first].m_interface->m_slider_value.get_value()>test) m_vertices[it->first].m_interface->m_top_box.set_border_color(ROUGE);
            if (m_vertices[it->first].m_interface->m_slider_value.get_value()<test) m_vertices[it->first].m_interface->m_top_box.set_border_color(VERT);
            if (m_vertices[it->first].m_interface->m_slider_value.get_value()==test) m_vertices[it->first].m_interface->m_top_box.set_border_color(ORANGECLAIR);

      m_vertices[it->first].m_value = test;

    }
    }
    else
    {
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            m_vertices[it->first].m_interface->m_top_box.set_moveable(true);
            m_vertices[it->first].m_interface->m_top_box.set_border_color(GRISSOMBRE);
        }

    }
    }
///BFS récursif
    void Graph::BFS(int NumS)
    {
            m_vertices[NumS].m_interface->m_top_box.set_mark(1);
        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
            if (it->second.m_from==NumS && m_vertices[it->second.m_to].m_interface->m_top_box.get_mark()==0)
            {   std::cout<<"\n"<<it->second.m_to;
                BFS(it->second.m_to);

            }

        }




    }
   ///BFS sur le graphe inverse pour trouver composantes fortement connexes
    void Graph::BFS_inverse(int NumS)
    {

            m_vertices[NumS].m_interface->m_top_box.set_reverse_mark(1);

        for (auto it = m_reverse_edges.begin(); it!=m_reverse_edges.end(); ++it)
        {
            if (it->second.m_from==NumS && m_vertices[it->second.m_to].m_interface->m_top_box.get_reverse_mark()!=1)
            {
                std::cout<<"\n"<<it->second.m_to;

                BFS_inverse(it->second.m_to);
            }
        }

    }
/// algo vu en cours et réadapté pour trouver les composantes fortement connexes
    void Graph::FindComposanteFortementConnexe()
    {
        srand(time(NULL));
        int r,v,b;
        bool good = true;

        for (auto it = m_edges.begin(); it!=m_edges.end(); ++it)
        {
            m_reverse_edges[it->first].m_from=m_edges[it->first].m_to;
            m_reverse_edges[it->first].m_to=m_edges[it->first].m_from;
        }

        int num_connexe = 0;
        while(good ==true)
        {
            good=false;
        for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {
            if(m_vertices[it->first].m_interface->m_top_box.get_Num_Connexe()==0) good = true;
        }


             for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {


            if(m_vertices[it->first].m_interface->m_top_box.get_Num_Connexe()==0)
            {
                std::cout<<"\n------"<<it->first<<"-------\n";

            BFS(it->first);
            std::cout<<"\n------"<<it->first<<"-------\n";
/// Attribution même couleur aléatoire aux meme composantes d'un groupe
            BFS_inverse(it->first);
            num_connexe++;
            r=rand()%255;
            v=rand()%255;
            b=rand()%255;



            for (auto it = m_vertices.begin(); it!=m_vertices.end(); ++it)
        {

            if (m_vertices[it->first].m_interface->m_top_box.get_mark()==1 &&  m_vertices[it->first].m_interface->m_top_box.get_reverse_mark()==1)
            {
                m_vertices[it->first].m_interface->m_top_box.set_Num_Connexe(num_connexe);
                m_vertices[it->first].m_interface->m_top_box.set_bg_color(makecol(r,v,b));

            }
            }

        }
        for (auto ut = m_vertices.begin(); ut!=m_vertices.end(); ++ut)
        {
            m_vertices[ut->first].m_interface->m_top_box.set_mark(0);
            m_vertices[ut->first].m_interface->m_top_box.set_reverse_mark(0);
        }


    }
        }
    for (auto ut = m_vertices.begin(); ut!=m_vertices.end(); ++ut)
    {
       std::cout<<m_vertices[ut->first].m_interface->m_top_box.get_Num_Connexe()<<"  ";
       std::ostringstream strs;
    strs << m_vertices[ut->first].m_interface->m_top_box.get_Num_Connexe();
    std::string str = strs.str();
       m_vertices[ut->first].m_interface->m_label_grp.set_message(str);
       m_vertices[ut->first].m_interface->m_top_box.set_Num_Connexe(0);
    }
    std::cout<<"\n";
    }
