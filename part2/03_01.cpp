//@compile clang++ part2/03_01.cpp -std=c++2b -Wall -Wextra -Wpedantic
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>

struct Rectangle {
    int x1, y1, x2, y2 = 0;
    
    int area() const {
        return (x2 - x1) * (y2 - y1);
    }
    
    bool is_valid() const {
        return x1 <= x2 && y1 <= y2;
    }
};

int intersection_area(const std::vector<Rectangle>& rects) {
    if (rects.empty()) return 0;
    
    int left   = rects[0].x1;
    int top    = rects[0].y1;
    int right  = rects[0].x2;
    int bottom = rects[0].y2;
    
    for (size_t i = 1; i < rects.size(); ++i) {
        left   = std::max(left,   rects[i].x1);
        top    = std::max(top,    rects[i].y1);
        right  = std::min(right,  rects[i].x2);
        bottom = std::min(bottom, rects[i].y2);
        
        if (left >= right || top >= bottom) return 0;
    }
    
    return (right - left) * (bottom - top);
}

Rectangle bounding_box(const std::vector<Rectangle>& rects) {
    if (rects.empty()) return {0, 0, 0, 0};
    
    int left   = std::numeric_limits<int>::max();
    int top    = std::numeric_limits<int>::max();
    int right  = std::numeric_limits<int>::min();
    int bottom = std::numeric_limits<int>::min();
    
    for (const auto& rect : rects) {
        left   = std::min(left,   rect.x1);
        top    = std::min(top,    rect.y1);
        right  = std::max(right,  rect.x2);
        bottom = std::max(bottom, rect.y2);
    }
    
    return {left, top, right, bottom};
}

void print_rectangle(const Rectangle& rect, const std::string& name = "") {
    if (!name.empty()) {
        std::cout << name << ": ";
    }
    std::cout << "(" << rect.x1 << ", " << rect.y1 << ") - (" 
              << rect.x2 << ", " << rect.y2 << ")" << std::endl;
}

/*=================================================TEST=================================================*/
void test_intersection_area() {
    std::cout << "=== intersection_area Tests ===" << std::endl;
    
    // Test 1: Single rectangle
    {
        std::vector<Rectangle> rects = {{0, 0, 10, 10}};
        int area = intersection_area(rects);
        std::cout << "Single rectangle: area = " << area << std::endl;
        assert(area == 100);
    }
    
    // Test 2: Two intersecting rectangles
    {
        std::vector<Rectangle> rects = {{0, 0, 10, 10}, {5, 5, 15, 15}};
        int area = intersection_area(rects);
        std::cout << "Two intersecting rectangles: area = " << area << std::endl;
        assert(area == 25); // Intersection is 5x5
    }
    
    // Test 3: Two non-intersecting rectangles
    {
        std::vector<Rectangle> rects = {{0, 0, 5, 5}, {10, 10, 15, 15}};
        int area = intersection_area(rects);
        std::cout << "Two non-intersecting rectangles: area = " << area << std::endl;
        assert(area == 0);
    }
    
    // Test 4: Three rectangles with common intersection
    {
        std::vector<Rectangle> rects = {{0, 0, 10, 10}, {2, 2, 8, 8}, {3, 3, 7, 7}};
        int area = intersection_area(rects);
        std::cout << "Three rectangles with common intersection: area = " << area << std::endl;
        assert(area == 16); // Intersection is 4x4
    }
    
    // Test 5: Empty vector
    {
        std::vector<Rectangle> rects;
        int area = intersection_area(rects);
        std::cout << "Empty vector: area = " << area << std::endl;
        assert(area == 0);
    }
    
    // Test 6: Degenerate rectangle (zero width)
    {
        std::vector<Rectangle> rects = {{0, 0, 0, 10}, {0, 0, 5, 5}};
        int area = intersection_area(rects);
        std::cout << "Degenerate rectangle: area = " << area << std::endl;
        assert(area == 0);
    }
    
    // Test 7: Touching rectangles
    {
        std::vector<Rectangle> rects = {{0, 0, 5, 5}, {5, 0, 10, 5}};
        int area = intersection_area(rects);
        std::cout << "Touching rectangles: area = " << area << std::endl;
        assert(area == 0); // Touching is not considered intersection
    }
    
    std::cout << "All intersection_area tests passed!\n" << std::endl;
}

void test_bounding_box() {
    std::cout << "=== bounding_box Tests ===" << std::endl;
    
    // Test 1: Single rectangle
    {
        std::vector<Rectangle> rects = {{2, 3, 8, 7}};
        Rectangle bbox = bounding_box(rects);
        std::cout << "Single rectangle: ";
        print_rectangle(bbox);
        assert(bbox.x1 == 2 && bbox.y1 == 3 && bbox.x2 == 8 && bbox.y2 == 7);
    }
    
    // Test 2: Two rectangles
    {
        std::vector<Rectangle> rects = {{0, 0, 5, 5}, {3, 3, 10, 8}};
        Rectangle bbox = bounding_box(rects);
        std::cout << "Two rectangles: ";
        print_rectangle(bbox);
        assert(bbox.x1 == 0 && bbox.y1 == 0 && bbox.x2 == 10 && bbox.y2 == 8);
    }
    
    // Test 3: Three rectangles
    {
        std::vector<Rectangle> rects = {{1, 2, 4, 6}, {0, 0, 3, 3}, {2, 1, 5, 4}};
        Rectangle bbox = bounding_box(rects);
        std::cout << "Three rectangles: ";
        print_rectangle(bbox);
        assert(bbox.x1 == 0 && bbox.y1 == 0 && bbox.x2 == 5 && bbox.y2 == 6);
    }
    
    // Test 4: Empty vector
    {
        std::vector<Rectangle> rects;
        Rectangle bbox = bounding_box(rects);
        std::cout << "Empty vector: ";
        print_rectangle(bbox);
        assert(bbox.x1 == 0 && bbox.y1 == 0 && bbox.x2 == 0 && bbox.y2 == 0);
    }
    
    // Test 5: Rectangles in different quadrants
    {
        std::vector<Rectangle> rects = {{-5, -5, -1, -1}, {1, 1, 5, 5}};
        Rectangle bbox = bounding_box(rects);
        std::cout << "Rectangles in different quadrants: ";
        print_rectangle(bbox);
        assert(bbox.x1 == -5 && bbox.y1 == -5 && bbox.x2 == 5 && bbox.y2 == 5);
    }
    
    // Test 6: Degenerate rectangle (zero width)
    {
        std::vector<Rectangle> rects = {{0, 0, 0, 10}, {0, 0, 5, 5}};
        Rectangle bbox = bounding_box(rects);
        std::cout << "Degenerate rectangle: ";
        print_rectangle(bbox);
        assert(bbox.x1 == 0 && bbox.y1 == 0 && bbox.x2 == 5 && bbox.y2 == 10);
    }

    std::cout << "All bounding_box tests passed!\n" << std::endl;
}

int main() {
    // Run tests
    test_intersection_area();
    test_bounding_box();
        
    std::cout << "All tests completed successfully!" << std::endl;
    
    return 0;
}