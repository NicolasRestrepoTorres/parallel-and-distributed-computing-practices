#include <opencv2/opencv.hpp>
using namespace cv;


int main()
{
    Mat1b grayImg = imread("path_to_image", IMREAD_GRAYSCALE);

    Mat1f data(grayImg.rows*grayImg.cols, 1);
    for (int r = 0; r < grayImg.rows; r++)
    {
        for (int c = 0; c < grayImg.cols; c++)
        {
            data(r*grayImg.cols + c) = float(grayImg(r, c));

        }
    }

    // Or, equivalently
    //Mat1f data;
    //grayImg.convertTo(data, CV_32F);
    //data = data.reshape(1, 1).t();


    // I should have obtained the vector in p, so now I want to supply it to k-means:
    int K = 8;
    Mat1i bestLabels(data.size(), 0); // integer matrix of labels
    Mat1f centers;                    // float matrix of centers
    cv::kmeans(data, K, bestLabels,
        cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
        3, cv::KMEANS_PP_CENTERS, centers);


    // Show results
    Mat1b result(grayImg.rows, grayImg.cols);
    for (int r = 0; r < result.rows; ++r)
    {
        for (int c = 0; c < result.cols; ++c)
        {
            result(r, c) = static_cast<uchar>(centers(bestLabels(r*grayImg.cols + c)));
        }
    }

    imshow("Image", grayImg);
    imshow("Result", result);
    waitKey();

    return 0;
}
