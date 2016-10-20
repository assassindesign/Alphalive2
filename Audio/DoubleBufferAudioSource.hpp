//
//  DoubleBufferAudioSource.hpp
//  iAlpha
//
//  Created by Luke Saxton on 13/11/2015.
//
//

#ifndef DoubleBufferAudioSource_hpp
#define DoubleBufferAudioSource_hpp

#include "Constants.h"
//r#include "AudioBufferSource.hpp"
#include "MasterClock.hpp"

/**
    Complex file streaming audio source, with loop quantisation and delayed triggering. ported from iAlpha, so may not be working 100%

 */
class DoubleBufferAudioSource : public AudioSource,
                                public ChangeListener,
                                public MasterClock::Listener
{
public:
    enum BufferType
    {
        Foreground = 0,
        Background,
    };

    enum ReadHead
    {
        HeadA = 0,
        HeadB,
        HeadC
    };

    struct FileInfo
    {
        int beats = 0;
        int position = 0;
        bool endsBeforeBeat = false;
        bool endsAfterBeat = false;

    };

    DoubleBufferAudioSource(TimeSliceThread* sharedTimeSlice, MasterClock* masterClock, ValueTree positionToUpdate);
    //DoubleBufferAudioSource(TimeSliceThread* sharedTimeSlice);

    ~DoubleBufferAudioSource();


    //===Audio Source===========================================
    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    void prepareToPlay(int samplesPerBlock, double sampleRate) override;
    void releaseResources() override;
    bool isPlaying() const;
    bool loadFileIntoBuffer(File fileToLoad, BufferType bufferToSet);
    void changeListenerCallback (ChangeBroadcaster *source) override;

    //===Master Clock Listener===========================================
    void barClockCallback() override;
    void stepClockCallback(const int currentPositionInLoop) override;
    void masterClockStopped() override;
    void masterTempoChanged(const int beatsInLoop, const float newTempo) override;

    //===Internal===============================================

//    int64 getTotalLength() const override;
//
//    void setNextReadPosition(int64 newPosition) override;
//
//    int64 getNextReadPosition() const override;

    bool isLooping() const;
    void setLooping(bool shouldLoop);
    void setPlaying(bool newState);

    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void sourceStartedOrStopped(DoubleBufferAudioSource* changedSource) = 0;
    private:
    };

    void addListener(DoubleBufferAudioSource::Listener* const listenerToAdd);
    void removeListener(DoubleBufferAudioSource::Listener* const listenerToRemove);
    void setGain(const float newGain);

private:

    void commonInit();
    void start();
    void stop();
    void start(ReadHead readerToStart);
    void stop(ReadHead readerToStop);

    AudioFormatReader* getAudioFormatReader(BufferType bufferToGet) const;
    AudioFormatReaderSource* getAudioFormatReaderSource(BufferType bufferToGet) const;
    AudioTransportSource* getAudioTransportSource(BufferType bufferToGet) const;

    bool shouldBePlaying, playing, looping, shouldStart, shouldStop;
    float tempo;

    ReadHead currentReadHead;

    MixerAudioSource mixer;

    OwnedArray<AudioTransportSource> transportSources;
    OwnedArray<AudioFormatReaderSource> formatReaderSources;
    OwnedArray<AudioFormatReader> formatReaders;
    OwnedArray<FileInfo> fileInfo;

    ValueTree positionData;

    TimeSliceThread* timeSliceThread;

    ListenerList<DoubleBufferAudioSource::Listener> listeners;

};
#endif /* DoubleBufferAudioSource_hpp */
