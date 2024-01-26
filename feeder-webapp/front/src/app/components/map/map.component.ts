import {Component, Input, OnChanges, OnInit} from '@angular/core';
import * as Leaflet from 'leaflet';
import {Feeder} from '../../interfaces/feeder';
import {BehaviorSubject} from 'rxjs';
import {DragEndEvent, LeafletMouseEvent} from 'leaflet';
import {FeederService} from '../../services/feeder.service';
import {LoaderService} from '../../services/loader.service';
import {BsModalService} from 'ngx-bootstrap/modal';
import {FeederEntryComponent} from '../feeder-entry/feeder-entry.component';

@Component({
  selector: 'app-map',
  templateUrl: './map.component.html',
  styleUrls: ['./map.component.scss']
})
export class MapComponent implements OnChanges, OnInit {
  @Input() feeders!: Feeder[];

  DEFAULT_LATITUDE = 47.261479;
  DEFAULT_LONGITUDE = -1.549479;

  constructor(
    private feederService: FeederService,
    private loaderService: LoaderService,
    private modalService: BsModalService
  ) {
  }

  ngOnInit(): void {
    this.feederService.feederDeleted.subscribe((feeder: Feeder) => {
      if (feeder) {
        this.generateMarkers();
      }
    });

    this.feederService.feederUpdated.subscribe((feeder: Feeder) => {
      if (feeder) {
        const index = this.feeders.findIndex((f: Feeder) => f.id === feeder.id);
        if (index !== -1) {
          this.feeders[index] = feeder;
        }
        this.generateMarkers();
      }
    });

  }

  mapLoaded: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);
  map!: Leaflet.Map;
  markers: Leaflet.Marker[] = [];
  markerGroup = Leaflet.layerGroup();
  options = {
    layers: [
      Leaflet.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>'
      })
    ],
    zoom: 0
  }

  ngOnChanges(): void {
    this.mapLoaded.subscribe((loaded: boolean) => {
      if (loaded) {
        this.generateMarkers();
      }
    });
  }

  addFeederToMap(feeder: Feeder): void {
    const position = { lat: feeder.latitude, lng: feeder.longitude };
    const marker = this.generateMarker(position, feeder);
    //marker.addTo(this.map);
    marker.addTo(this.markerGroup);
    this.markers.push(marker);
  }

  generateMarkers() {
    this.markerGroup.clearLayers();
    if (this.feeders && (this.feeders.length > 0)) {
      this.feeders.forEach((feeder: Feeder) => {
        this.addFeederToMap(feeder);
      });

      // @ts-ignore
      const group = new Leaflet.featureGroup(this.markers);

      this.map.fitBounds(group.getBounds());

    }
    else {
      this.map.setView({ lat: this.DEFAULT_LATITUDE, lng: this.DEFAULT_LONGITUDE}, 16);
    }
    this.map.addLayer(this.markerGroup);
  }

  generateMarker(position: any, feeder: Feeder): Leaflet.Marker {
    const feederIcon = Leaflet.icon({iconUrl: 'assets/img/feeder-map.png'});
    return Leaflet.marker(position, {title: feeder.description, icon: feederIcon, draggable: true})
      .on('click', (event) => this.markerClicked(event, feeder.id))
      .on('dragend', (event) => this.markerDragEnd(event, feeder.id));
  }

  displayFeederEntry(longitude: number, latitude: number): void {
    let nextId = 1;
    if (this.feeders.length > 0) {
      nextId =  Math.max(...this.feeders.map(feeder => feeder.id)) + 1;
    }

    const feeder: Feeder = {
      id: nextId,
      description: "",
      latitude: latitude,
      longitude: longitude,
      birdFiles: [],
      sensorMeasurements: []
    }
    const initialState = {
      feeder: {...feeder},
      creation: true
    };

    const modalRef = this.modalService.show(FeederEntryComponent, {initialState});
    // @ts-ignore
    modalRef.content.onValidate.subscribe((feeder: Feeder) => {
      console.log('feeder created', feeder);
      this.feederService.addFeeder(feeder).subscribe(
        (feeder: Feeder) => {
          if (feeder) {
            console.log('Feeder created');
            this.feeders.push(feeder);
            this.addFeederToMap(feeder);
          }
          else {
            console.error('Error during feeder creation'); // TODO toast
          }
        }
      )
    });
  }

  onMapReady($event: Leaflet.Map): void {
    this.map = $event;
    this.mapLoaded.next(true);
  }

  markerClicked($event: LeafletMouseEvent, index: number): void {
    console.log($event.latlng.lat, $event.latlng.lng);
    const feeder: Feeder | undefined = this.feeders.find(f => f.id === index);
    console.log('Click on ' + feeder?.description);
  }

  onMapClick($event: LeafletMouseEvent): void {
    const longitude = $event.latlng.lng;
    const latitude = $event.latlng.lat;

    this.displayFeederEntry(longitude, latitude);
  }

  markerDragEnd($event: DragEndEvent, index: number): void {
    console.log($event.target.getLatLng());
    const latLng = $event.target.getLatLng();
    const feeder: Feeder | undefined = this.feeders.find(f => f.id === index);
    if (feeder) {
      feeder.longitude = latLng.lng;
      feeder.latitude = latLng.lat;
      this.loaderService.showValidation();
      this.feederService.updateFeeder(feeder).subscribe({
          next: result => {
            if (!result) {
              console.error('Mise à jour impossible'); // TODO
            }
            this.loaderService.hide();
          },
          error: err => {
            console.error('Erreur lors de la mise à jour impossible', err); // TODO
            this.loaderService.hide();
          }
      }
      )
    }
    console.log('End drag of feeder ' + feeder?.description);
  }

}
