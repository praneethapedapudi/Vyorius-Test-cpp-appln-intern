#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

// Function to load the ratings matrix from a CSV file
vector<vector<int>> loadRatingsMatrix(const string &filename) {
    ifstream file(filename);
    vector<vector<int>> matrix;
    string line;

    bool skipHeader=true; // Flag to skip the first row (header)
    while(getline(file, line)) {
        if(skipHeader){ 
            skipHeader=false;
            continue; // Skip the header row
        }
        vector<int>row;
        stringstream ss(line);
        string value;

        while(getline(ss, value, ',')){
            row.push_back(value.empty()?0:stoi(value)); // Handle empty values
        }
        matrix.push_back(row);
    }
    file.close();
    return matrix;
}

// Function to compute cosine similarity between two users
double calculateCosineSimilarity(const vector<int> &user1, const vector<int> &user2) {
    double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;

    for (size_t i = 0; i < user1.size(); ++i) {
        if (user1[i] != 0 && user2[i] != 0) { // Only consider rated movies
            dotProduct += user1[i] * user2[i];
            norm1 += user1[i] * user1[i];
            norm2 += user2[i] * user2[i];
        }
    }

    if (norm1 == 0 || norm2 == 0) return 0; // Avoid division by zero
    return dotProduct / (sqrt(norm1) * sqrt(norm2));
}

// Function to calculate all user-user similarities
vector<double> calculateAllSimilarities(const vector<vector<int>> &matrix, int targetUser) {
    vector<double> similarities(matrix.size(), 0.0);

    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i != targetUser) {
            similarities[i] = calculateCosineSimilarity(matrix[targetUser], matrix[i]);
        }
    }
    return similarities;
}

// Function to predict ratings for a target user
vector<double> predictRatings(const vector<vector<int>> &matrix, int targetUser) {
    int numMovies = matrix[0].size();
    vector<double> predictedRatings(numMovies, 0.0);
    vector<double> similarities = calculateAllSimilarities(matrix, targetUser);

    for (int movie = 0; movie < numMovies; ++movie) {
        if (matrix[targetUser][movie] == 0) { // Predict only for unrated movies
            double numerator = 0.0, denominator = 0.0;

            for (size_t user = 0; user < matrix.size(); ++user) {
                if (matrix[user][movie] != 0 && similarities[user] > 0) {
                    numerator += similarities[user] * matrix[user][movie];
                    denominator += fabs(similarities[user]);
                }
            }

            predictedRatings[movie] = (denominator != 0) ? numerator / denominator : 0;
        }
    }

    return predictedRatings;
}

// Function to recommend top N movies for a user
void recommendTopNMovies(const vector<double> &predictions, int N) {
    vector<pair<double, int>> movies;

    for (int i = 0; i < predictions.size(); ++i) {
        if (predictions[i] > 0) {
            movies.push_back({predictions[i], i + 1}); // Movie index starts at 1
        }
    }

    // Sort movies by predicted ratings in descending order
    sort(movies.rbegin(), movies.rend());

    // Print top N movies
    cout << "Top " << N << " Recommended Movies:\n";
    for (int i = 0; i < N && i < movies.size(); ++i) {
        cout << "Movie " << movies[i].second << " - Predicted Rating: " << movies[i].first << endl;
    }
}

// Function to calculate RMSE (Root Mean Square Error)
double calculateRMSE(const vector<vector<int>> &matrix, const vector<double> &predictions, int targetUser) {
    double error = 0.0;
    int count = 0;

    for (size_t i = 0; i < predictions.size(); ++i) {
        if (matrix[targetUser][i] != 0) { // Compare only for rated movies
            double diff = predictions[i] - matrix[targetUser][i];
            error += diff * diff;
            count++;
        }
    }

    return(count>0)?sqrt(error/count):0.0;
}

int main() {
    string filename="ratings.csv"; // CSV file with user-movie ratings

    // User input for target user and number of recommended movies
    int targetUser, top, temp;

    cout << "Enter the user index for which you want to recommend movies: ";
    cin >> temp;
    targetUser=temp-1;

    cout<<"Enter the number of top recommended movies: ";
    cin>>top;

    // Load the ratings matrix
    vector<vector<int>> ratingsMatrix=loadRatingsMatrix(filename);

    // Predict ratings for the target user
    vector<double> predictedRatings = predictRatings(ratingsMatrix, targetUser);

    // Print predicted ratings
    cout<<"Predicted Ratings for User "<<targetUser+1<<":\n";
    for(int i=0; i<predictedRatings.size(); i++) {
        if(ratingsMatrix[targetUser][i]==0){ // Show only unrated movies
            cout<<"Movie "<<i+1<<": "<<predictedRatings[i]<<endl;
        }
    }

    // Recommend top n movies
    recommendTopNMovies(predictedRatings, top);

    // Calculate and print RMSE
    double rmse=calculateRMSE(ratingsMatrix, predictedRatings, targetUser);
    cout<<"\nPerformance Report:\n";
    cout<<"RMSE for User "<<targetUser+1<<": "<<rmse<<endl;

    return 0;
}
