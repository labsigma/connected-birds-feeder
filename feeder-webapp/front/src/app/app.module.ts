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
import {HttpClient, HttpClientModule} from '@angular/common/http';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import { MapComponent } from './components/map/map.component';
import {LeafletModule} from '@asymmetrik/ngx-leaflet';
import {NgOptimizedImage} from '@angular/common';
import {TranslateLoader, TranslateModule} from '@ngx-translate/core';
import {ModalModule} from 'ngx-bootstrap/modal';
import {TranslateHttpLoader} from '@ngx-translate/http-loader';
import { FeederEntryComponent } from './components/feeder-entry/feeder-entry.component';
import {FormsModule} from '@angular/forms';
import {TranslationModule} from './core/translation.module';
import { ConfirmationComponent } from './components/confirmation/confirmation.component';
import {ToastrModule} from 'ngx-toastr';

@NgModule({
  declarations: [
    AppComponent,
    FeedersComponent,
    FeederComponent,
    GalleryComponent,
    BirdComponent,
    LoaderComponent,
    MapComponent,
    FeederEntryComponent,
    ConfirmationComponent
  ],
  imports: [
    BrowserAnimationsModule,
    BrowserModule,
    AppRoutingModule,
    NgxSpinnerModule,
    HttpClientModule,
    LeafletModule,
    NgOptimizedImage,
    TranslateModule,
    TranslationModule,
    ModalModule.forRoot(),
    FormsModule,
    ToastrModule.forRoot(
      {
        timeOut: 5000,
        positionClass: 'toast-top-center'
      }
    )
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

export function HttpLoaderFactory(http: HttpClient): TranslateHttpLoader {
  return new TranslateHttpLoader(http);
}
