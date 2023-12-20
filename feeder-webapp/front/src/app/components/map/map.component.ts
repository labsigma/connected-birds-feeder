import {Component, Input, OnChanges, OnInit, SimpleChanges} from '@angular/core';
import * as Leaflet from 'leaflet';
import {Feeder} from '../../interfaces/feeder';
import {BehaviorSubject} from 'rxjs';
import {LeafletMouseEvent} from 'leaflet';

@Component({
  selector: 'app-map',
  templateUrl: './map.component.html',
  styleUrls: ['./map.component.scss']
})
export class MapComponent implements OnChanges {
  @Input() feeders!: Feeder[];

  mapLoaded: BehaviorSubject<boolean> = new BehaviorSubject<boolean>(false);
  map!: Leaflet.Map;
  markers: Leaflet.Marker[] = [];
  options = {
    layers: [
      Leaflet.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a>'
      })
    ],
    zoom: 0
  }

  ngOnChanges(changes: SimpleChanges): void {
    this.mapLoaded.subscribe((loaded: boolean) => {
      if (loaded) {
        this.initMarkers();
      }
    });
  }

  initMarkers() {
    if (this.feeders) {
      this.feeders.forEach((feeder: Feeder) => {
        const position = { lat: feeder.latitude, lng: feeder.longitude };
        const marker = this.generateMarker(position, feeder);
        marker.addTo(this.map);
        this.markers.push(marker);
      });

      // @ts-ignore
      const group = new Leaflet.featureGroup(this.markers);

      this.map.fitBounds(group.getBounds());
    }

  }

  generateMarker(position: any, feeder: Feeder): Leaflet.Marker {
    const feederIcon = Leaflet.icon({iconUrl: 'assets/img/feeder.png'});
    return Leaflet.marker(position, {title: feeder.description, icon: feederIcon, draggable: true})
      .on('click', (event) => this.markerClicked(event, feeder.id));
  }

  onMapReady($event: Leaflet.Map): void {
    this.map = $event;
    this.mapLoaded.next(true);
  }

  markerClicked($event: any, index: number): void {
    console.log($event.latlng.lat, $event.latlng.lng);
    const feeder: Feeder | undefined = this.feeders.find(f => f.id === index);
    console.log('Clik on ' + feeder?.description);
  }

  onMapClick($event: LeafletMouseEvent): void {
    console.log($event.latlng.lat, $event.latlng.lng);
  }
}
