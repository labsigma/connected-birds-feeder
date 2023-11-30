import base64
import json
import os

from PIL import Image
from flask import request, Flask
from keras.preprocessing.image import load_img
from resnet_model import ResnetModel
from flask_httpauth import HTTPBasicAuth
from flask_restful import Api
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime

img_width = os.getenv("IMAGE_WIDTH") or 398
img_height = os.getenv("IMAGE_HEIGHT") or 398
language = os.getenv("LANGUAGE") or 'fr'

USERNAME = os.getenv("CREDENTIAL_USERNAME")
PASSWORD = os.getenv("CREDENTIAL_PASSWORD")

bucket = os.getenv("INFLUXDB_BUCKET")
influxdb_url = os.getenv("INFLUXDB_URL")
influxdb_token = os.getenv("INFLUXDB_TOKEN")
influxdb_org = os.getenv("ORG")
client = InfluxDBClient(url=influxdb_url, token=influxdb_token, org=influxdb_org)
write_api = client.write_api(write_options=SYNCHRONOUS)


def load_index_to_label_dict() -> dict:
    """Retrieves and formats the
    index to class label
    lookup dictionary needed to
    make sense of the predictions.
    When loaded in, the keys are strings, this also
    processes those keys to integers."""
    path = './index_to_class_label/' + language + '.json'
    with open(path, 'r') as f:
        index_label = json.load(f)
    index_label = {
        int(k): v for k, v in index_label.items()}
    return index_label


def load_model(path: str = 'models/trained_model_resnet50.pt') -> ResnetModel:
    """Retrieves the trained model and maps it to the CPU by default,
    can also specify GPU here."""
    return ResnetModel(path_to_pretrained_model=path)


model = load_model()
index_to_class_label_dict = load_index_to_label_dict()


def predict(
        img: Image.Image,
        index_to_label_dict: dict,
        model_ia,
        k: int
) -> list:
    """Transforming input image according to ImageNet paper
    The Resnet was initially trained on ImageNet dataset
    and because of the use of transfer learning, I froze all
    weights and only learned weights on the final layer.
    The weights of the first layer are still what was
    used in the ImageNet paper, and we need to process
    the new images just like they did.

    This function transforms the image accordingly,
    puts it to the necessary device (cpu by default here),
    feeds the image through the model getting the output tensor,
    converts that output tensor to probabilities using Softmax,
    and then extracts and formats the top k predictions."""
    formatted_predictions = model_ia.predict_proba(img, k, index_to_label_dict)
    return formatted_predictions


def classify_bird(image_path: str) -> list:
    print(image_path)
    image = load_img(image_path, target_size=(img_width, img_height), interpolation='lanczos')
    prediction = predict(image, index_to_class_label_dict, model, k=5)
    print(prediction)
    return prediction


def send_to_influxdb(
        id_feeder: int,
        file_name: str,
        bird_name: str,
        percent_prediction: str
):
    print('send value to influxdb')
    p = (Point("feeders")
         .tag("feeder_id", "IDFEEDER_" + str(id_feeder))
         .field("file", file_name)
         .field("bird_name", bird_name)
         .field('percent_prediction', percent_prediction))
    write_api.write(bucket=bucket, record=p)


application = Flask(__name__)
auth = HTTPBasicAuth()
api: Api = Api(app=application, serve_challenge_on_401=True, decorators=[auth.login_required])


@auth.verify_password
def verify_password(username: str, password: str):
    if username == USERNAME and password == PASSWORD:
        return username


def extract_base64_string(image_data):
    # Split the image data into the prefix and base64 string
    prefix, base64_string = image_data.split(",")

    # Return the base64 string without the prefix
    return base64_string


@application.route('/api/pictures', methods=['POST'])
@auth.login_required
def analyse_image():
    json_body = request.get_json()
    print(json_body)

    now = datetime.now()  # current date and time

    id_feeder = json_body.get('idFeeder')
    file_name = "IDFEEDER_" + str(id_feeder) + "_ " + now.strftime("%Y%m%d-%H%M%S") + ".jpg"

    # Make sure the upload directoy is created
    upload_directory = './uploads/' + str(id_feeder)
    os.makedirs(upload_directory, exist_ok=True)

    extracted_base64_string = extract_base64_string(json_body.get('picture'))

    with open(upload_directory + '/' + file_name, 'wb') as fh:
        fh.write(base64.urlsafe_b64decode(extracted_base64_string))

    prediction = classify_bird(upload_directory + '/' + file_name)
    print(prediction)

    bird_name = 'Unknown'
    percent_prediction = ''
    if len(prediction) > 0:
        bird_name = prediction[0][0]
        percent_prediction = prediction[0][1]

    if id_feeder is not None:
        send_to_influxdb(
            id_feeder=id_feeder,
            file_name=file_name,
            bird_name=bird_name,
            percent_prediction=percent_prediction
        )

    return json.dumps(prediction)


@application.route('/health', methods=['GET'])
def life_probe():
    print('influxdb_org : ' + influxdb_org)
    print('influxdb_url : ' + influxdb_url)
    print('influxdb_token : ' + influxdb_token)
    return 'ia-recognition api is running...'


application.run(host='0.0.0.0', debug=True, port=5001)
