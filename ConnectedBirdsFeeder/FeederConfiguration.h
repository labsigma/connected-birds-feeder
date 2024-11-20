#define SEND_TO_GED false
#define DEBUG_MODE true

#define MEASUREMENT_INTERVAL 10 // Time ESP32 will go to sleep (in seconds)
#define ID_FEEDER 1 // Identifier of the feeder. It must be unique for all connected feeders that send their data to the same endpoint
#define DHT_PIN 21   // Pin on which is connected the DHT22 (temperature and humidity sensor)
#define DHT_TYPE DHT22   // DHT sensor type
#define MQ135_PIN 47 // Pin on which the MQ135 is connected (pollution sensor)

// PIN on whith the presence detector module is connected.
#define PIR_PIN  GPIO_NUM_14 //  Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.

#define WIFI_SSID "lelieumultiple"
#define WIFI_PASSWORD "Boop>^Azo4l,q|6MG_j,U\"<HH\@"

#define INFLUXDB_URL "http://dav.rabouin.es:8086"
#define INFLUXDB_TOKEN "my-super-secret-auth-token"
#define INFLUXDB_ORG "lelieumultiple"
#define INFLUXDB_BUCKET "birdy"

#define HTTP_TIMEOUT 30000

#define IA_RECOGNITION_API "http://dav.rabouin.es:5001/api/pictures"
#define IA_RECOGNITION_USERNAME "my-user"
#define IA_RECOGNITION_PASSWORD "my-password"

#define GED_API "https://recette-supplychainmobile.joinmyit.com/mobilite-mo-ws/creerfichier"
#define GED_USERNAME "6e7b7b2f-dc66-4fb4-94cc-834572f6c55b"
#define GED_PASSWORD "11ef5e9c-87c9-4a2a-964a-cb540243f2e4"


// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE // Has PSRAM
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM

