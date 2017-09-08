#ifndef LEPTON3
#define LEPTON3

#include <ctime>
#include <stdint.h>

#include <iostream>
#include <cstdlib>
#include <thread>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "LEPTON_Types.h"

#include "stopwatch.hpp"

class Lepton3
{
public:
    typedef enum _debug_lvl { DBG_NONE=0, DBG_INFO=1, DBG_FULL=2 } DebugLvl;

    Lepton3(std::string spiDevice="/dev/spidev1.0", uint16_t cciPort=1, DebugLvl dbgLvl=DBG_FULL);
    ~Lepton3();

    bool start();
    void stop();

    unsigned short* getLastFrame();

protected:
    void thread_func();

    bool SpiOpenPort();     //!< Open VoSPI port
    void SpiClosePort();    //!< Close VoSPI port
    int SpiReadSegment();   //!< Read a new VoSPI segment and returns its ID
    void resync();          //!< Resync VoSPI communication after de-sync

    void raw2data();        //!< Convert RAW VoSPI frame to 16bit data matrix

private:
    // >>>>> VoSPI
    std::string mSpiDevice; //!< SPI port device name
    int mSpiFd;             //!< SPI descriptor
    unsigned char mSpiMode; //!< SPI mode
    unsigned char mSpiBits; //!< SPI bits per words
    unsigned int mSpiSpeed; //!< SPI max speed

    uint8_t mPacketCount;   //!< VoSPI Packet for each segment
    uint8_t mPacketSize;    //!< VoSPI Packet size in bytes
    uint8_t mSegmentCount;  //!< VoSPI segment for each frame
    uint32_t mSegmSize;     //!< Size of the single segment

    uint8_t* mSpiRawFrameBuf;      //!< VoSPI buffer to keep 4 consecutive valid segments
    uint32_t mSpiRawFrameBufSize;  //!< Size of the buffer for 4 segments

    uint16_t* mDataFrameBuf;      //!< RAW 16bit frame buffer

    double mSegmentFreq;    //!< VoSPI Segment output frequency
    
    struct spi_ioc_transfer mSpiTR; //!< Data structure for SPI ioctl

    int mCurrSegm;          //!< Index of the last valid acquired segment (-1 if Lost Sync)
    // <<<<< VoSPI

    // >>>>> Lepton control (CCI)
    bool mCciConnected;
    LEP_UINT16 mCciPort;
    LEP_CAMERA_PORT_DESC_T_PTR mCciConnPort;

    bool CciConnect();                  //!< Connect CCI (I2C)
    bool lepton_perform_ffc();          //!< Force FFC calculation
    float getSensorTemperatureK();      //!< Get Temperature of the Flir Sensor in °K
    float raw2Celsius(float);           //!< Converts a RAW value to °C
    int enableRadiometry( bool enable );//!< Enable/Disable radiometry
    // <<<<< Lepton control (CCI)

    std::thread mThread;

    bool mStop;

    DebugLvl mDebugLvl;
    
    StopWatch mThreadWatch;

    bool mDataValid;
};



#endif // LEPTONTHREAD
