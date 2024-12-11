# Movie Recommendation System

This project implements a **Movie Recommendation System** using **collaborative filtering** in C++. The system predicts the ratings of unrated movies for a specific user based on similarities with other users and recommends the top N movies. It also calculates **Root Mean Squared Error (RMSE)** to evaluate the accuracy of the predictions.

## Features

- **Collaborative Filtering**: Predicts movie ratings based on user similarities using **cosine similarity**.
- **Top N Recommendations**: Recommends the top N movies based on predicted ratings.
- **RMSE Calculation**: Calculates **Root Mean Squared Error** to evaluate prediction accuracy.

## Setup and Usage

### 1. Generate Random Ratings Data

Generate the `ratings.csv` file containing random ratings for users and movies using Python. This data will be used to test the recommendation system.

### 2. Compile and Run the C++ Program

1. **Save the C++ code** in a file and compile it using a C++ compiler.
2. **Run the compiled program**, which will prompt you for:
   - The **target user** for whom you want to predict ratings.
   - The **top N recommended movies** you want to display.
3. The program will output the predicted ratings of unrated movies, the top N recommended movies, and the **RMSE** for evaluation.
