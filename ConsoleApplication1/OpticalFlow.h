#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class OpticalFlow {
	public:
		// Funcions
		OpticalFlow();
		void drawArrow(Mat image, Point p, Point q, Scalar color, int arrowMagnitude, int thickness, int line_type, int shift);
		Mat calculaPuntsClau(Mat frame1, Mat frame2);
		void CannyThreshold(int, void*);
		Mat aplicarCanny(Mat frame1, Mat frame1_d);
		Mat calcularMascara(Mat frame_d);
		Point3f transformaCoordenades(Point3f p, int width, int height);
		Mat calcularOpticalFlow3D(Mat& frame1, Mat& frame2, Mat frame1_d, Mat frame2_d);
		int getSize();
		vector<Point3i> getOpticalFlow3DInici();
		vector<Point3i> getOpticalFlow3DDespl();
		float calculaModul(Point3i p);
			
	private:
		// Atributs
		vector<Point3i> OpticalFlow3DInici;
		vector<Point3i> OpticalFlow3DDespl;
		int size;
		Point3i origen;
};