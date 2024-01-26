import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {FeedersComponent} from './components/feeders/feeders.component';
import {GalleryComponent} from './components/gallery/gallery.component';

const routes: Routes = [
  {path: '', redirectTo:'/feeders', pathMatch: 'full' },
  {
    path: 'feeders',
    component: FeedersComponent
  },
  {path: 'gallery', component: GalleryComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule { }
