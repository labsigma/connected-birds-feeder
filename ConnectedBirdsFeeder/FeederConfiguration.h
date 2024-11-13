#define SEND_TO_GED false
#define DEBUG_MODE true

#define MEASUREMENT_INTERVAL 10 // Time ESP32 will go to sleep (in seconds)
#define ID_FEEDER 1 // Identifier of the feeder. It must be unique for all connected feeders that send their data to the same endpoint
#define DHT_PIN 2   // Pin on which is connected the DHT22 (temperature and humidity sensor)
#define DHT_TYPE DHT22   // DHT sensor type
#define MQ135_PIN 12
 // Pin on which the MQ135 is connected (pollution sensor)

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


// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13

// PIN on whith the presence detector module is connected.
#define PIR_PIN  GPIO_NUM_14 //  Only RTC IO can be used as a source for external wake source. They are pins: 0,2,4,12-15,25-27,32-39.
