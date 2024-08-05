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

    for (tinyxml2::XMLElement* element = svgRoot->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        const char* tagName = element->Name();
        if (std::string(tagName) == "circle") {
            float cx = element->FloatAttribute("cx");
            float cy = element->FloatAttribute("cy");
            float r = element->FloatAttribute("r");
            std::vector<Point> circlePoints = sampleCircle(cx, cy, r);
            points.insert(points.end(), circlePoints.begin(), circlePoints.end());
        } else if (std::string(tagName) == "ellipse") {
            float cx = element->FloatAttribute("cx");
            float cy = element->FloatAttribute("cy");
            float rx = element->FloatAttribute("rx");
            float ry = element->FloatAttribute("ry");
            std::vector<Point> ellipsePoints = sampleEllipse(cx, cy, rx, ry);
            points.insert(points.end(), ellipsePoints.begin(), ellipsePoints.end());
        } else if (std::string(tagName) == "rect") {
            float x = element->FloatAttribute("x");
            float y = element->FloatAttribute("y");
            float width = element->FloatAttribute("width");
            float height = element->FloatAttribute("height");
            points.emplace_back(x, y);
            points.emplace_back(x + width, y);
            points.emplace_back(x + width, y + height);
            points.emplace_back(x, y + height);
        } else if (std::string(tagName) == "line") {
            float x1 = element->FloatAttribute("x1");
            float y1 = element->FloatAttribute("y1");
            float x2 = element->FloatAttribute("x2");
            float y2 = element->FloatAttribute("y2");
            points.emplace_back(x1, y1);
            points.emplace_back(x2, y2);
        } else if (std::string(tagName) == "polyline" || std::string(tagName) == "polygon") {
            const char* pointsAttr = element->Attribute("points");
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
        }
        // Path element handling would go here (complex parsing)
    }

    return points;
}