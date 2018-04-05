#include "graph.h"
#include <fstream>
#include <sstream>
#include "grman/widget.h"


/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_value.set_dim(20,80);
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


    ///TECHNIQUE POUR CONVERTIR SANS LA FONCTION RELOU
    std::ostringstream strs;
    strs << idx;
    std::string str = strs.str();
     m_label_idx.set_message( str );
}


/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    std::ostringstream strs;
    strs << m_value;
    std::string str = strs.str();

   m_interface->m_label_value.set_message( str );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
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

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(18,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0 , 100); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(12,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider

    std::ostringstream strs;
    strs << m_weight;
    std::string str = strs.str();
    m_interface->m_label_weight.set_message( str ) ;
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/


/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
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

    m_top_box.add_child(m_checkbox);
    m_checkbox.set_dim(50,40);
    m_checkbox.set_pos(20,500);

    ///Bouton Sauvegarde
    m_top_box.add_child(m_button1);
    m_button1.set_dim(50,40);
    m_button1.set_pos(15,10);
    m_button1.set_bg_color(BLEUCLAIR);
    m_button1.add_child(m_img_save);
    m_img_save.set_pic_name("save.jpg");


    m_top_box.add_child(m_button2);
    m_button2.set_dim(50,40);
    m_button2.set_pos(15,60);
    m_button2.add_child(m_text1);
    m_text1.set_message("Supr Sommet");

    m_top_box.add_child(m_button3);
    m_button3.set_dim(50,40);
    m_button3.set_pos(15,210);
    m_button3.add_child(m_text2);
    m_text2.set_message("Add Arete");

    m_top_box.add_child(m_button4);
    m_button4.set_dim(50,40);
    m_button4.set_pos(15,110);
    m_button4.add_child(m_text3);
    m_text3.set_message("Supr Arete");

    m_top_box.add_child(m_button5);
    m_button5.set_dim(50,40);
    m_button5.set_pos(15,160);
    m_button5.add_child(m_text4);
    m_text4.set_message("Add Sommet");


}


  /* void Graph::loadFile(){
      std::string file = "save.txt";
     std::ifstream fichier(file, std::ios::in);  // on ouvre
     if(fichier){

             int ordre;
            fichier >> ordre;

            int id;
            float value;
            int x;
            int y;
            std::string content;

            for(int i = 0; i < ordre; i++){
                fichier >> id ;
                fichier >> value;
                fichier >> x;
                fichier >> y;
                fichier >> content;

                ajoutSommet(id,value,x,y,content);
            }

            fichier.close();

     }
     else{
             std::cout << "Erreur chargement du fichier \n";


        }

     }
     */







///Ss programme pour lire un fichier et remplir un graphe
void Graph::loadFile(std::string fileName)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    ///varialbe string pour sauter des lignes
    std::string nom;
    ///information utile pour faire le sommet
    int numS, x, y;
    double valeur;
    std::string pic_name;
    ///lecture du fichier
    std::ifstream fichier(fileName+".txt",std::ios::in);
    if(fichier)
    {

       // int m_ordre;
       // int m_nbArete;
        ///on r�cup�re le premier caract�re puis on saute une ligne
        fichier>>m_ordre;
        getline(fichier,nom);
        ///on remplit notre vector int�rm�diare en fonction de l'ordre du graphe avec des 0
        for(int i=0; i<m_ordre; i++)
        {
            fichier>>numS>>valeur>>x>>y>>pic_name;
            //getline(fichier,nom);

            add_interfaced_vertex(numS,valeur,x,y,pic_name);
        }
        fichier>>m_nbArete;
        std::cout<<m_nbArete;
        getline(fichier,nom);
        ///Adjacences
        for(int i=0; i<m_nbArete; i++)
        {
            fichier>>numS>>x>>y>>valeur;

            add_interfaced_edge(numS,x,y,valeur);
        }

        fichier.close();
    }

    else
    {
        std::cout<<"Il n'exite pas de fichier portant ce nom";
    }
   // std::cout<<m_vertices[3].m_interface->m_top_box.get_posx();
}

void Graph::saveFile(std::string fileNameSave)
{

    // std::cout<<m_vertices[3].m_interface->m_top_box.get_posx();
    std::ofstream fichier(fileNameSave, std::ios::out);  // ouverture en �criture avec
    if (fichier)
    {
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
           ///d�part
           fichier << it->second.m_from<<" ";
           ///arriv�e
           fichier << it-> second.m_to<<" ";
           /// poids
           fichier << it-> second.m_weight<<"\n";


        }
        fichier.close();
    }
         else
                std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
}



/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
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

/// Aide � l'ajout de sommets interfac�s
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}
///fichier << m_vertices[i].m_interface->m_top_box.get_posx();

/// Aide � l'ajout d'arcs interfac�s
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

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}
/// eidx index of edge to remove
void Graph::test_remove_edge(int eidx)
{
/// r�f�rence vers le Edge � enlever
Edge &remed=m_edges.at(eidx);


std::cout << "Removing edge " << eidx << " " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

/*/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
std::cout << m_edges.size() << std::endl;*/

/// test : on a bien des �l�ments interfac�s
if (m_interface && remed.m_interface)
{
/// Ne pas oublier qu'on a fait �a � l'ajout de l'arc :
/* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
/* m_interface->m_main_box.add_child(ei->m_top_edge); */
/* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne n�cessite pas de delete car on a un shared_ptr
/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
}

/// Il reste encore � virer l'arc supprim� de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
std::vector<int> &veto = m_vertices[remed.m_to].m_in;
vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne n�cessite pas non plus de delete car on n'a pas fait de new (s�mantique par valeur)
/// Il suffit donc de supprimer l'entr�e de la map pour supprimer � la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
m_edges.erase( eidx );

/*/// Tester la coh�rence : nombre d'arc entrants et sortants des sommets 1 et 2
std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
std::cout << m_edges.size() << std::endl;*/

//m_nbArete--;

}

void Graph::remove_vertex(int vidx)
{
    /// r�f�rence vers le Edge � enlever
    Vertex &supr=m_vertices.at(vidx);

    ///suppresion des arretes li�es � ce sommet
    for(int i = supr.m_in.size() -1 ; i >=0 ; i--)
        test_remove_edge(supr.m_in[i]);
    for(int i =supr.m_out.size() -1 ; i >=0  ; i--)
        test_remove_edge(supr.m_out[i]);

        ///on check les �l�ments interfac�s -> go delete if oui
        if (m_interface && supr.m_interface)
        {
            m_interface->m_main_box.remove_child( supr.m_interface->m_top_box );
        }

        ///on le supprime de la map
        m_vertices.erase(vidx);
        //m_ordre--;

}

void Graph::add_vertex()
{
    add_interfaced_vertex(m_ordre,30,mouse_x,mouse_y,"phoque.jpg");
    m_ordre++;

}

void Graph::add_edge(int from, int to)
{

    add_interfaced_edge(m_nbArete,from,to,30);
    m_nbArete++;


}
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
if(TuttoBene = -1)
{ return (-1);}
}


void Graph::animations()
{
///Sauvegarde
    if (m_interface->m_button1.clicked()) saveFile("save1.txt") ;

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
        std::cout<<Idx<<Idx2;
        }
        if(mouse_b&2) ok = true;

        }

    }


    ///Supression arete

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
             if(m_vertices[Idx].m_out[Idx2])
            {
                std::cout<<"OKOKOKOKKOKOKO";
               // m_vertices[Idx].m_out[Idx2]
        ok = true;
        test_remove_edge(3);
        std::cout<<Idx<<Idx2;
        }
        if(mouse_b&2) ok = true;
    }
    }
    }


    if (key[KEY_D])
    {
        add_vertex();
         while(key[KEY_D])
          {

          }
    }

    if (key[KEY_T])
    {

        test_remove_edge(m_nbArete-1);
         while(key[KEY_T])
          {

          }
    }



    }
