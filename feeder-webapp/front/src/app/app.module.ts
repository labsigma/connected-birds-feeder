import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { FeedersComponent } from './components/feeders/feeders.component';
import { FeederComponent } from './components/feeder/feeder.component';
import { GalleryComponent } from './components/gallery/gallery.component';
import { BirdComponent } from './components/bird/bird.component';
import {NgxSpinnerModule} from 'ngx-spinner';
import { LoaderComponent } from './components/loader/loader.component';
import {HttpClientModule} from '@angular/common/http';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import { MapComponent } from './components/map/map.component';
import {LeafletModule} from '@asymmetrik/ngx-leaflet';
import {NgOptimizedImage} from '@angular/common';

@NgModule({
  declarations: [
    AppComponent,
    FeedersComponent,
    FeederComponent,
    GalleryComponent,
    BirdComponent,
    LoaderComponent,
    MapComponent
  ],
    imports: [
        BrowserAnimationsModule,
        BrowserModule,
        AppRoutingModule,
        NgxSpinnerModule,
        HttpClientModule,
        LeafletModule,
        NgOptimizedImage
    ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
