﻿
#include "Constraints.hpp"
#include "Engine.hpp"

float Constraints::PercentageConstraint(float percentage, const std::string &edge, bool ignore_stack) {
    float screenWidth = MC::windowSize.x;
    float screenHeight = MC::windowSize.y;
    float x = 0;
    float y = 0;

    // Check if there is a dimension stack and use the top element's dimensions instead
    if (!FlarialGUI::dimensionStack.empty() && !ignore_stack) {
        Dimensions &top = FlarialGUI::dimensionStack.top();
        screenWidth = top.width;
        screenHeight = top.height;
        x = top.x;
        y = top.y;
    }

    float position;

    if (edge == "top") {
        position = percentage * screenHeight;
        position += y;
    } else if (edge == "bottom") {
        position = screenHeight - (percentage * screenHeight);
        position += y;
    } else if (edge == "left") {
        position = percentage * screenWidth;
        position += x;
    } else if (edge == "right") {
        position = screenWidth - (percentage * screenWidth); 
        position += x;
    }

    return std::ceilf(position * 10000.0f) / 10000.0f;
}

float Constraints::RelativeConstraint(float percent, const std::string &dimension, bool ignore_stack) {
    float length;
    float screenWidth;
    float screenHeight;

    if (ignore_stack || FlarialGUI::dimensionStack.empty()) {
        screenWidth = MC::windowSize.x;
        screenHeight = MC::windowSize.y;
    } else {
        screenWidth = FlarialGUI::dimensionStack.top().width;
        screenHeight = FlarialGUI::dimensionStack.top().height;
    }


    if (dimension == "width") {
        length = screenWidth * percent;
    } else if (dimension == "height") {
        length = screenHeight * percent;
    }

    return length;
}

Vec2<float>
Constraints::CenterConstraint(float width, float height, const std::string &axis, float xModifier, float yModifier, bool ignoreStack) {
    Vec2<float> xy;

    float screenWidth;
    float screenHeight;
    float parentX = 0;
    float parentY = 0;

    if (ignoreStack || FlarialGUI::dimensionStack.empty()) {
        screenWidth = MC::windowSize.x;
        screenHeight = MC::windowSize.y;
    } else {
        screenWidth = FlarialGUI::dimensionStack.top().width;
        screenHeight = FlarialGUI::dimensionStack.top().height;
        parentX = FlarialGUI::dimensionStack.top().x;
        parentY = FlarialGUI::dimensionStack.top().y;
    }

    xy.x = (screenWidth - width) / 2.0f + parentX;
    xy.y = (screenHeight - height) / 2.0f + parentY;

    if (axis == "x") {
        xy.x += xModifier * (screenWidth - width) / 2.0f;
    } else if (axis == "y") {
        xy.y += yModifier * (screenHeight - height) / 2.0f;
    } else if (axis == "both") {
        xy.x += xModifier * (screenWidth - width) / 2.0f;
        xy.y += yModifier * (screenHeight - height) / 2.0f;
    }

    return xy;
}

Vec2<float> Constraints::RoundingConstraint(float radiusX, float radiusY) {
    // Get the minimum screen dimension

    float screenHeight = MC::windowSize.y;

    // Calculate the scaled radii
    float scaledRadiusX = radiusX * screenHeight / 1920.0f;
    float scaledRadiusY = radiusY * screenHeight / 1080.0f;

    // Return the scaled radii as a Vec2
    return Vec2<float>{scaledRadiusX, scaledRadiusY};
}

float Constraints::FontScaler(float dimension) {

    // Define the initial font size
    float initialFontSize = 14.0f;

// Calculate the scaling factor based on the new window dimensions
    float scalingFactor = dimension / 100.0f;

// Calculate the new font size
    return initialFontSize * scalingFactor;

}

Vec2<float> Constraints::CalculatePercentage(float x, float y, float adjustWidth, float adjustHeight) {
    return Vec2<float>{x / (MC::windowSize.x - adjustWidth), y / (MC::windowSize.y - adjustHeight)};
}


float Constraints::SpacingConstraint(float percentage, float dimension) {

    return dimension * percentage;

}