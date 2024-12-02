# GameOfLife
//le jeu de la vie en C++

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <thread>

using namespace std;

class GameOfLife {
private:
    int rows, cols;
    vector<vector<bool>> grid;

public:
    // Constructeur
    GameOfLife(int rows, int cols) : rows(rows), cols(cols), grid(rows, vector<bool>(cols, false)) {}

    // Charger la grille initiale depuis un fichier
    void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
        exit(1);
    }

    // Lire les dimensions
    file >> rows >> cols;

    // Redimensionner la grille
    grid.assign(rows, vector<bool>(cols, false));

    // Lire les cellules présentes dans le fichier
    int i = 0;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Ignorer les lignes vides

        // Lire les cellules ligne par ligne
        int j = 0;
        for (char c : line) {
            if (c == '0' || c == '1') {
                if (i < rows && j < cols) {
                    grid[i][j] = (c == '1');
                }
                j++;
            }
        }
        i++;
    }

    // Si moins de lignes ou colonnes fournies, elles restent initialisées à `0`
    // grâce à `grid.assign(rows, vector<bool>(cols, false))`
}


    // Affichage de la grille dans la console
    void display() const {
        // Efface l'écran
        #ifdef _WIN32
            system("cls"); // Commande pour Windows
        #else
            system("clear"); // Commande pour Linux/Mac
        #endif

        // Affiche la grille
        for (const auto& row : grid) {
            for (bool cell : row) {
                cout << (cell ? "O" : ".") << " ";
            }
            cout << endl;
        }
    }

    // Enregistrer la grille dans un fichier
    void writeToFile(const string& filename, int iteration) const {
        ofstream outFile(filename, ios::app); // Ouvrir en mode ajout (append)
        if (!outFile) {
            cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
            return;
        }

        outFile << "Iteration " << iteration << ":\n";
        for (const auto& row : grid) {
            for (bool cell : row) {
                outFile << (cell ? "1" : "0") << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";
        outFile.close();
    }

    // Calculer la génération suivante
    void computeNextGeneration() {
        vector<vector<bool>> nextGrid = grid;

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int liveNeighbors = countLiveNeighbors(i, j);

                if (grid[i][j]) {
                    // Cellule vivante
                    nextGrid[i][j] = (liveNeighbors == 2 || liveNeighbors == 3);
                } else {
                    // Cellule morte
                    nextGrid[i][j] = (liveNeighbors == 3);
                }
            }
        }

        grid = nextGrid;
    }

    // Compter les voisins vivants d'une cellule
    int countLiveNeighbors(int x, int y) const {
        static const vector<pair<int, int>> directions = {
            {-1, -1}, {-1, 0}, {-1, 1},
            { 0, -1},          { 0, 1},
            { 1, -1}, { 1, 0}, { 1, 1}
        };

        int count = 0;
        for (const auto& dir : directions) {
            int nx = x + dir.first;
            int ny = y + dir.second;
            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny]) {
                count++;
            }
        }
        return count;
    }

    // Méthode principale pour lancer le jeu
    void run(int iterations, int delayMs, const string& outputFilename) {
        // Supprimer le fichier existant (si présent)
        ofstream clearFile(outputFilename, ios::trunc);
        clearFile.close();

        for (int i = 0; i < iterations; i++) {
            display();
            writeToFile(outputFilename, i);
            computeNextGeneration();
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
    }
};

// Programme principal
int main() {
    std::string filename = "grille.txt";

    // Création d'une instance de GameOfLife
    GameOfLife game(0, 0);

    // Charger la grille initiale depuis le fichier
    game.loadFromFile(filename);

    // Lancer le jeu pour 100 générations avec un délai de 200 ms
    game.run(100, 200, "result.txt");

    return 0;
}

