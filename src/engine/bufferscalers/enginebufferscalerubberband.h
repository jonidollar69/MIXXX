#pragma once

#include "engine/bufferscalers/enginebufferscale.h"
#include "util/memory.h"

namespace RubberBand {
class RubberBandStretcher;
}  // namespace RubberBand

class ReadAheadManager;

// Uses librubberband to scale audio.  This class is not thread safe.
class EngineBufferScaleRubberBand : public EngineBufferScale {
    Q_OBJECT
  public:
    explicit EngineBufferScaleRubberBand(
            ReadAheadManager* pReadAheadManager);
    ~EngineBufferScaleRubberBand() override;

    // Let EngineBuffer know if engine v3 is available
    static bool isEngineFinerAvailable();

    // Enable engine v3 if available
    void useEngineFiner(bool enable);

    void setScaleParameters(double base_rate,
                            double* pTempoRatio,
                            double* pPitchRatio) override;

    double scaleBuffer(
            CSAMPLE* pOutputBuffer,
            SINT iOutputBufferSize) override;

    // Flush buffer.
    void clear() override;

  private:
    // Reset RubberBand library with new audio signal
    void onSampleRateChanged() override;

    int runningEngineVersion();

    void deinterleaveAndProcess(const CSAMPLE* pBuffer, SINT frames, bool flush);
    SINT retrieveAndDeinterleave(CSAMPLE* pBuffer, SINT frames);

    // The read-ahead manager that we use to fetch samples
    ReadAheadManager* m_pReadAheadManager;

    std::unique_ptr<RubberBand::RubberBandStretcher> m_pRubberBand;

    CSAMPLE* m_retrieve_buffer[2];
    CSAMPLE* m_buffer_back;

    // Holds the playback direction
    bool m_bBackwards;

    bool m_useEngineFiner;
};
