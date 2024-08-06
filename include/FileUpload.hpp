#include <vector>
#include <Point.hpp>
#include <tinyxml2.h>

std::vector<Point> sampleCircle(float cx, float cy, float r, int numSamples = 100) {
    std::vector<Point> points;
    for (int i = 0; i < numSamples; ++i) {
        float angle = 2 * M_PI * i / numSamples;
        points.emplace_back(cx + r * cos(angle), cy + r * sin(angle));
    }
    return points;
}

std::vector<Point> sampleEllipse(float cx, float cy, float rx, float ry, int numSamples = 100) {
    std::vector<Point> points;
    for (int i = 0; i < numSamples; ++i) {
        float angle = 2 * M_PI * i / numSamples;
        points.emplace_back(cx + rx * cos(angle), cy + ry * sin(angle));
    }
    return points;
}


void extractPointsFromElement(tinyxml2::XMLElement* element, std::vector<Point>& points) {
    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        const char* tagName = child->Name();
        if (std::string(tagName) == "circle") {
            std::cout << "Circle" << std::endl;
            float cx = child->FloatAttribute("cx");
            float cy = child->FloatAttribute("cy");
            float r = child->FloatAttribute("r");
            std::vector<Point> circlePoints = sampleCircle(cx, cy, r);
            points.insert(points.end(), circlePoints.begin(), circlePoints.end());
        } else if (std::string(tagName) == "ellipse") {
            std::cout << "Ellipse" << std::endl;
            float cx = child->FloatAttribute("cx");
            float cy = child->FloatAttribute("cy");
            float rx = child->FloatAttribute("rx");
            float ry = child->FloatAttribute("ry");
            std::vector<Point> ellipsePoints = sampleEllipse(cx, cy, rx, ry);
            points.insert(points.end(), ellipsePoints.begin(), ellipsePoints.end());
        } else if (std::string(tagName) == "rect") {
            std::cout << "Rectangle" << std::endl;
            float x = child->FloatAttribute("x");
            float y = child->FloatAttribute("y");
            float width = child->FloatAttribute("width");
            float height = child->FloatAttribute("height");
            points.emplace_back(x, y);
            points.emplace_back(x + width, y);
            points.emplace_back(x + width, y + height);
            points.emplace_back(x, y + height);
        } else if (std::string(tagName) == "line") {
            std::cout << "Line" << std::endl;
            float x1 = child->FloatAttribute("x1");
            float y1 = child->FloatAttribute("y1");
            float x2 = child->FloatAttribute("x2");
            float y2 = child->FloatAttribute("y2");
            points.emplace_back(x1, y1);
            points.emplace_back(x2, y2);
        } else if (std::string(tagName) == "polyline" || std::string(tagName) == "polygon") {
            std::cout << "Poly" << std::endl;
            const char* pointsAttr = child->Attribute("points");
            if (pointsAttr) {
                std::string pointsStr = pointsAttr;
                std::stringstream ss(pointsStr);
                std::string point;
                while (std::getline(ss, point, ' ')) {
                    std::stringstream pointStream(point);
                    std::string coord;
                    float coords[2];
                    int i = 0;
                    while (std::getline(pointStream, coord, ',')) {
                        coords[i++] = std::stof(coord);
                    }
                    points.emplace_back(coords[0], coords[1]);
                }
            }
        } else {
            extractPointsFromElement(child, points);
        }
    }
}

std::vector<Point> extractPointsFromSVG(const char* svgFile) {
    std::vector<Point> points;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(svgFile) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load SVG file!" << std::endl;
        return points;
    }

    tinyxml2::XMLElement* svgRoot = doc.FirstChildElement("svg");
    if (!svgRoot) {
        std::cerr << "No <svg> element found in the file!" << std::endl;
        return points;
    }

    // recursivly traverse
    extractPointsFromElement(svgRoot, points);

    return points;
}