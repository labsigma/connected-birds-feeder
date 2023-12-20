import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {FeedersComponent} from './components/feeders/feeders.component';
import {GalleryComponent} from './components/gallery/gallery.component';

const routes: Routes = [
  {path: '', redirectTo:'/feeders', pathMatch: 'full' },
  {path: 'feeders', component: FeedersComponent, title: 'Connected bird feeders'},
  {path: 'gallery', component: GalleryComponent, title: 'Gallery of birds'}
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule { }
