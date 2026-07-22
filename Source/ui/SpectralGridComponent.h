#pragma once

#include <JuceHeader.h>

#include <array>
#include <functional>
#include <vector>

class SpectralGridComponent final : public juce::Component
{
public:
    static constexpr int kNumBands = 3;
    static constexpr int kNumLayers = 3;
    static constexpr int kNumCells = kNumBands * kNumLayers;
    static constexpr int kWaveTableSize = 513;

    SpectralGridComponent();

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;
    void mouseDrag (const juce::MouseEvent&) override;
    void mouseUp (const juce::MouseEvent&) override;

    void setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data);
    void setSelectedCell (int cellIndex);
    int getSelectedCell() const { return selectedCell; }

    void setVerticalLine (int index, float normalizedPos);
    void setHorizontalLine (int index, float normalizedPos);
    float getVerticalLine (int index) const;
    float getHorizontalLine (int index) const;

    void setAnalyzerData (const std::vector<float>& magnitudes);
    void setCellActivity (const std::array<float, kNumCells>& activity);

    std::function<void (int cellIndex)> onCellClicked;
    std::function<void (int lineIndex, float normalizedPos)> onVerticalLineDragged;
    std::function<void (int lineIndex, float normalizedPos)> onHorizontalLineDragged;

private:
    struct CellInfo
    {
        std::array<float, kWaveTableSize> shapeData {};
        juce::Rectangle<float> bounds;
    };

    std::array<CellInfo, kNumCells> cells;
    int selectedCell = -1;

    std::array<float, 2> vLinePos = { 0.33f, 0.66f };
    std::array<float, 2> hLinePos = { 0.33f, 0.66f };

    enum class DragTarget { None, VLine0, VLine1, HLine0, HLine1, Intersection00, Intersection01, Intersection10, Intersection11, Cell };
    DragTarget dragTarget = DragTarget::None;
    int dragCellIndex = -1;
    float dragStartX = 0, dragStartY = 0;

    std::vector<float> analyzerData;
    std::array<float, kNumCells> cellActivity {};
    juce::Rectangle<int> gridBounds;

    void recalcGridBounds();
    void drawShape (juce::Graphics& g, juce::Rectangle<float> bounds, const std::array<float, kWaveTableSize>& data, float alpha);

    float getCellLeft (int band) const;
    float getCellRight (int band) const;
    float getCellTop (int layer) const;
    float getCellBottom (int layer) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralGridComponent)
};
