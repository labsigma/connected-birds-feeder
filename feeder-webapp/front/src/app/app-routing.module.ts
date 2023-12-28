import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import {FeedersComponent} from './components/feeders/feeders.component';
import {GalleryComponent} from './components/gallery/gallery.component';
import {CustomTitleResolver} from './custom-title.resolver';

const routes: Routes = [
  {path: '', redirectTo:'/feeders', pathMatch: 'full' },
  {
    path: 'feeders',
    component: FeedersComponent,
    title: CustomTitleResolver,
    data: {
      titleKey: 'titre.page-principal'
    }
  },
  {path: 'gallery', component: GalleryComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes, { useHash: true })],
  exports: [RouterModule]
})
export class AppRoutingModule { }
