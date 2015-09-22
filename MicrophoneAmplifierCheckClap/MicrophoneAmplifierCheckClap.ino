//  ------------------------------------------------------------------------------------------------
//  ------------------------------------------------------------------------------------------------
#define PWM_MAX_VALUE                   1024
#define PWM_MIN_VALUE                   0

//  ------------------------------------------------------------------------------------------------
//  ------------------------------------------------------------------------------------------------

const   byte                            pinLED      = 13;
const   byte                            pinCheckMic = A5;

static  unsigned int                    checkCycleMillisecond   = 1000;
static  unsigned int                    checkMicVoiceAmplitude  = 300;


static  unsigned long                   tickSystem;
static  unsigned int                    minMicVoice;
static  unsigned int                    maxMicVoice;
static  bool                            stateLED;



//  ------------------------------------------------------------------------------------------------
//  ------------------------------------------------------------------------------------------------
void setup()
{
    tickSystem                          = PWM_MIN_VALUE;
    minMicVoice                         = PWM_MAX_VALUE;
    maxMicVoice                         = 0;
    stateLED                            = false;

    pinMode( pinLED, OUTPUT );

    Serial.begin( 9600 );
}


//  ------------------------------------------------------------------------------------------------
void loop()
{
    unsigned long                       nowTick;
    unsigned int                        voiceMic;

    nowTick                             = millis();                     //  get system millisecond;
    voiceMic                            = analogRead( pinCheckMic );    //  get microphone voice value.
    minMicVoice                         = min ( minMicVoice, voiceMic );
    maxMicVoice                         = max ( voiceMic, maxMicVoice );

    //  check a clap in a second.
    if ( ( nowTick - tickSystem ) < checkCycleMillisecond )
    {
        if ( ( maxMicVoice - minMicVoice ) < checkMicVoiceAmplitude )
        {
            return;
        }

        Serial.print( "a voice distance of clap : " );
        Serial.println( ( maxMicVoice - minMicVoice ) );

        digitalWrite( pinLED, stateLED );

        minMicVoice                     = PWM_MAX_VALUE;
        maxMicVoice                     = PWM_MIN_VALUE;
        stateLED                        = !stateLED;

        //  when get the clap signal, delay the check.
        delay( 100 );
        return;
    }

    tickSystem                          = nowTick;
}

//  ------------------------------------------------------------------------------------------------

