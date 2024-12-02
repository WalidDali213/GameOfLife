#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
 
using namespace std;
 
// Classe GameOfLife
class GameOfLife {
private:
    int rows; // Nombre de lignes
    int cols; // Nombre de colonnes
    vector<vector<int>> grid; // Grille actuelle
    vector<vector<int>> nextGrid; // Grille suivante
 
public:
    // Constructeur
    GameOfLife(int rows, int cols) : rows(rows), cols(cols), grid(rows, vector<int>(cols, 0)), nextGrid(rows, vector<int>(cols, 0)) {}
 
    // Méthode pour initialiser la grille avec un modèle
    void initialize(const vector<pair<int, int>>& liveCells) {
        for (const auto& cell : liveCells) {
            grid[cell.first][cell.second] = 1;
        }
    }
 
    // Méthode pour afficher la grille
    void display() const {
        system("clear"); // Utilisez "cls" sous Windows
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << (grid[i][j] ? "O" : ".") << " ";
            }
            cout << endl;
        }
    }
 
    // Méthode pour calculer le nombre de voisins vivants
    int countAliveNeighbors(int x, int y) const {
        int count = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0) continue; // Ignorer la cellule elle-même
                int nx = x + i, ny = y + j;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                    count += grid[nx][ny];
                }
            }
        }
        return count;
    }
 
    // Méthode pour calculer la génération suivante
    void computeNextGeneration() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int aliveNeighbors = countAliveNeighbors(i, j);
 
                if (grid[i][j] == 1) { // Cellule vivante
                    if (aliveNeighbors < 2 || aliveNeighbors > 3) {
                        nextGrid[i][j] = 0; // Meurt
                    } else {
                        nextGrid[i][j] = 1; // Reste vivante
                    }
                } else { // Cellule morte
                    if (aliveNeighbors == 3) {
                        nextGrid[i][j] = 1; // Devient vivante
                    } else {
                        nextGrid[i][j] = 0; // Reste morte
                    }
                }
            }
        }
        // Mise à jour de la grille
        grid = nextGrid;
    }
 
    // Méthode principale pour lancer le jeu
    void run(int iterations, int delayMs) {
        for (int i = 0; i < iterations; i++) {
            display();
            computeNextGeneration();
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
    }
};
 
// Programme principal
int main() {
    std::string filename = "grille.txt";
 
    // Création d'un objet ifstream (input file stream) pour lire le fichier
    std::ifstream file(filename);
 
    // Vérifier si le fichier a été ouvert avec succès
    if (!file) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
        return 1; // Retourne une erreur
    }
 
 
    int rows, cols;
 
    file>>rows;
    file>>cols;
 
    // Création d'une instance de GameOfLife
    GameOfLife game(rows, cols);
 
    // Initialisation avec un modèle (planeur)
    game.initialize({
        {1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}
    });
 
    // Lancer le jeu pour 100 générations avec un délai de 200 ms
    game.run(100, 2000);
 
    return 0;
}
