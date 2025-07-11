#include <iostream>
#include <vector>
#include <array>

// Helper function: Compute the Longest Common Subsequence (LCS) for approximate matching
void longest_common_subsequence(const std::vector<int>& sub1, const std::vector<int>& sub2, int& longest_length, int& start_idx1, int& start_idx2) {
    std::vector<std::vector<int>> dp(sub1.size() + 1, std::vector<int>(sub2.size() + 1, 0));
    longest_length = 0;
    start_idx1 = -1;
    start_idx2 = -1;

    for (size_t i = 1; i <= sub1.size(); ++i) {
        for (size_t j = 1; j <= sub2.size(); ++j) {
            if (sub1[i - 1] == sub2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > longest_length) {
                    longest_length = dp[i][j];
                    start_idx1 = i - longest_length; // Update start index for sub1
                    start_idx2 = j - longest_length; // Update start index for sub2
                }
            }
        }
    }
}

// Function to detect exact matches in the submissions
void find_exact_matches(const std::vector<int>& submission1, const std::vector<int>& submission2, int& total_exact_length) {
    total_exact_length = 0;
    std::vector<bool> matched1(submission1.size(), false);
    std::vector<bool> matched2(submission2.size(), false);

    for (size_t i = 0; i < submission1.size(); ++i) {
        for (size_t j = 0; j < submission2.size(); ++j) {
            // Check for matches of length 10 to 20
            for (size_t len = 10; len <= 20; ++len) {
                if (i + len <= submission1.size() && j + len <= submission2.size()) {
                    bool is_match = true;
                    for (size_t k = 0; k < len; ++k) {
                        if (submission1[i + k] != submission2[j + k]) {
                            is_match = false;
                            break;
                        }
                    }
                    if (is_match) {
                        // Mark these indices as matched to avoid double counting
                        for (size_t k = 0; k < len; ++k) {
                            matched1[i + k] = true;
                            matched2[j + k] = true;
                        }
                        total_exact_length += len;
                    }
                }
            }
        }
    }
}

// Function: Detect plagiarism between two submissions
std::array<int, 5> match_submissions(std::vector<int>& submission1, std::vector<int>& submission2) {
    std::array<int, 5> result = {0, 0, 0, 0, 0}; // Default return values

    // Step 1: Exact pattern matching (10-20 tokens)
    int total_exact_length = 0;
    find_exact_matches(submission1, submission2, total_exact_length);

    // Step 2: Approximate matching (30+ tokens) using LCS
    int longest_approx_match = 0;
    int start_idx1 = -1, start_idx2 = -1;
    longest_common_subsequence(submission1, submission2, longest_approx_match, start_idx1, start_idx2);
    
    if (longest_approx_match < 30) {
        longest_approx_match = 0; // Ignore if below threshold
        start_idx1 = -1; // Reset start index if no valid match
        start_idx2 = -1; // Reset start index if no valid match
    }

    // Step 3: Populate the result array
    result[0] = (total_exact_length >= 50 || longest_approx_match >= 30) ? 1 : 0; // Plagiarism flag
    result[1] = total_exact_length; // Total exact match length
    result[2] = longest_approx_match; // Longest approximate match length
    result[3] = start_idx1; // Start index of the longest match in submission1
    result[4] = start_idx2; // Start index of the longest match in submission2

    return result;
}

int main() {
    // Sample tokenized submissions
    std::vector<int> submission1 = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    std::vector<int> submission2 = {0, 1, 10, 11, 12, 13, 50, 51, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};

    // Call the plagiarism checker function
    std::array<int, 5> result = match_submissions(submission1, submission2);

    // Print the results
    std::cout << "Plagiarism Flag: " << result[0] << std::endl;
    std::cout << "Total Exact Match Length: " << result[1] << std::endl;
    std::cout << "Longest Approximate Match Length: " << result[2] << std::endl;
    std::cout << "Start Index in Submission 1: " << result[3] << std::endl;
    std::cout << "Start Index in Submission 2: " << result[4] << std::endl;

    return 0;
}
