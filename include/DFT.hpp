#include <complex>
#include <Consts.hpp>
#include <algorithm>

using complex = std::complex<double>;

class Fourier {

public :
    Fourier() {
        N = 0;
    }

    Fourier(std::vector<complex> c) : coords(c) {
        N = coords.size();
    }

    Fourier(std::vector<complex> c, Point org) : coords(c), origin(org) {
        N = coords.size();
    }

    void setCoords(std::vector<complex> c) {
        coords = c;
        N = coords.size();
    }

    void DFT() {
        dftRes.clear();
        for (int k = 0; k < N; k++) {
            complex sum = complex(0, 0);
            for (int n = 0; n < N; n++) {
                double phi = (2 * Consts::PI * k * n) / N;
                complex c(cos(phi), sin(phi));
                complex res = coords[n] * c;
                sum = sum + res;
            }


            std::vector<double> res;
            res.push_back(sum.real() / (double) N);
            res.push_back(sum.imag() / (double) N);
            res.push_back(k);
            res.push_back(sqrt(std::norm(sum)) / (double) N);
            res.push_back(std::arg(sum));

            dftRes.push_back(res);
        }
    }

    static bool cmpRadius(const Cycloid &v1, const Cycloid &v2) {
        return v1.getRadius() > v2.getRadius(); // larger radius
    }

    static bool cmpFreq(const Cycloid &v1, const Cycloid &v2) {
        return abs(v1.getFreq()) < abs(v2.getFreq()); // lower frequency
    }

    static void sortByRadius(std::vector<Cycloid> &cycles) {
        std::sort(cycles.begin(), cycles.end(), Fourier::cmpRadius);
    }

    static void sortByFreq(std::vector<Cycloid> &cycles) {
        std::sort(cycles.begin(), cycles.end(), Fourier::cmpFreq);
    }


    std::vector<Cycloid> constructEpicycles() {
        std::vector<Cycloid> epicycles;
        Point prevPoint = origin;
        for (int i = 0; i < N; i++) {
            epicycles.push_back(
                    Cycloid(dftRes[i][3], ((prevPoint)), (dftRes[i][2] > N / 2 ? dftRes[i][2] - N : dftRes[i][2]),
                            dftRes[i][4]));
            prevPoint = epicycles[i].getEndPoint();
        }
        return epicycles;
    }

    std::vector<std::vector<double>> getRes() {
        return dftRes;
    }

private:
    std::vector<complex> coords;
    std::vector<std::vector<double>> dftRes; // [real, imaginary, freq, radius, phase angle]
    Point origin = Point(0, 0);
    size_t N;
};