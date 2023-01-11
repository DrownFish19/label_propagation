#include "network.h"
#include <iostream>
#include <sstream>

Network::Network() {
    numberOfNodes = 0;
}

bool Network::initialize(string inputPath) {
    try {


        std::ifstream in(inputPath);
        std::string line;

        // first loop to get the number of nodes
        while (getline(in, line)) {   //将in文件中的每一行字符读入到string line中
            std::stringstream ss(line); //使用string初始化stringstream
            int v1;
            int v2;
            //    int weight;
            ss >> v1;
            ss >> v2;

            v1++;
            v2++;

            int value = (v2 > v1 ? v2 : v1);

            if (value > numberOfNodes)
                numberOfNodes = value;

        }


        // now get the edges

        // node labels start from zero so we should initializr from 1 to n
        edges = new SparseMatrix<int>(numberOfNodes);

        nodes = new Node[numberOfNodes + 1];
        for (int i = 1; i <= numberOfNodes; ++i)
            nodes[i].label = i;

        in.clear();
        in.seekg(ios::beg);

        while (getline(in, line)) {   //将in文件中的每一行字符读入到string line中
            std::stringstream ss(line); //使用string初始化stringstream
            int v1;
            int v2;
            //    int weight;
            ss >> v1;
            ss >> v2;

            v1++;
            v2++;

            // indirected graph; having edge in both direction
            edges->set(v1, v2, 1);
            edges->set(v2, v1, 1);

            // add the corresponding degree
            nodes[v1].degree++;
            nodes[v2].degree++;
        }



        // close the reading file
        in.close();
    }
    catch (exception e) {
        cerr << e.what() << endl;
    }

    return true;

}

bool Network::computeNodeInfluence() {
//    srand(QTime::currentTime().second());
    float alpha = (float) rand() / RAND_MAX;
    for (int i = 1; i <= numberOfNodes; ++i) {
        nodes[i].nodeInfluence = nodes[i].degree; // NI(i) = Ks(i) + ...

        float tmpSum = 0;
        for (int i = 1; i <= numberOfNodes; ++i)
            tmpSum += (float) (nodes[i].kShell / nodes[i].degree);
        tmpSum *= alpha;

        nodes[i].nodeInfluence += tmpSum;

    }


    return true;
}

bool Network::computeLabelInfluence() {
    for (int i = 1; i <= numberOfNodes; ++i) {
        for (int j = 1; j <= numberOfNodes; ++j) {
            if (edges->get(i, j) != 0) // they are neighbors
                nodes[i].labelInfluence += (float) (nodes[j].nodeInfluence
                                                    / nodes[j].degree);
        }
    }


    return true;
}

bool Network::computeNewLabels() {
    for (int i = 1; i <= numberOfNodes; ++i) {
        float maxLi;
        int j, indexMaxLi;
        for (j = 1; j <= numberOfNodes; ++j) {
            if (edges->get(i, j) != 0) {
                maxLi = nodes[j].labelInfluence;
                indexMaxLi = j;
                break;
            }
        }
        for (; j <= numberOfNodes; ++j) {
            if (edges->get(i, j) != 0) {
                if (nodes[j].nodeInfluence > maxLi) {
                    maxLi = nodes[i].labelInfluence;
                    indexMaxLi = j;
                }

            }
        }

        nodes[i].newLabel = indexMaxLi; // finish
    }


    return true;
}

bool Network::writeResultsToFile(string resultPath) {
    fstream output;
    try {
        output.open(resultPath, ios::app);
        for (int i = 1; i <= numberOfNodes; i++)
            output << i << "\t" << nodes[i].newLabel << endl;
        output.flush();
        output.close();
        return true;
    }
    catch (exception ex) {
        cerr << ex.what() << endl;
    }
    return true;

}
