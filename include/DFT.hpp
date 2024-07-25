#include <complex>
#include <Consts.hpp>
#include <algorithm>

using complex = std::complex<double>;

class Fourier {

    public :
        Fourier() {
            N = 0;
        }
        Fourier(std::vector<complex> c): coords(c) {
            N = coords.size();
        }
        Fourier(std::vector<complex> c, Point org): coords(c) , origin(org){
            N = coords.size();
        }

        void setCoords(std::vector<complex> c){
            coords = c;
            N = coords.size();
        }

        void DFT(){
            dftRes.clear();
            for (int k=0; k<N; k++){
                complex sum = complex(0, 0);
                for (int n=0; n<N; n++){
                    double phi = (2 * Consts::PI * k * n) / N;
                    complex c(cos(phi), sin(phi));
                    complex res = coords[n] * c;
                    sum = sum + res;
                }


                std::vector<double> res;
                res.push_back(sum.real() / N);
                res.push_back(sum.imag() / N);
                res.push_back(k);
                res.push_back(sqrt(std::norm(sum)) / N);
                res.push_back(std::arg(sum));

                dftRes.push_back(res);
            }
        }

        static bool cmp(const std::vector<double>& v1, const std::vector<double> &v2){
            return v1[3] > v2[3];
        }

        void sortRes(){
            std::sort(dftRes.begin(), dftRes.end(), Fourier::cmp);
        }

        std::vector<Cycloid> constructEpicycles() {
            std::vector<Cycloid> epicycles;
            Point prevPoint = origin;
            for (int i=0; i<N; i++){
//                std::cout << dftRes.size() << std::endl;
//                for (double x : dftRes[0]){
//                    std::cout << x << std::endl;
//                }
//                if (dftRes[i][3] > 1e-30){
                epicycles.push_back(Cycloid(dftRes[i][3], ((prevPoint)) , (dftRes[i][2] > N/2 ? dftRes[i][2]-N : dftRes[i][2]), dftRes[i][4]));
                std::cout << epicycles[i];
                prevPoint = epicycles[i].getEndPoint();
                std::cout << " endpoint " << epicycles[i].getEndPoint() << std::endl;
//                }
            }
            return epicycles;
        }

        std::vector<std::vector<double>> getRes(){
            return dftRes;
        }

    private:
        std::vector<complex> coords;
        std::vector<std::vector<double>> dftRes; // [real, imaginary, freq, radius, phase angle]
        Point origin = Point(0, 0);
        size_t N;
};