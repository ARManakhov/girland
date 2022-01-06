import SparkMD5 from 'spark-md5';
import Vue from 'vue';
import Meta from 'vue-meta';
import App from './App.vue';
// import {makeServer} from './test-server'
// // if (process.env.NODE_ENV === "development") {
// //     import('./test-server').then(i => i.makeServer())
// // }
// makeServer()
Vue.config.productionTip = false;
Object.defineProperty(Vue.prototype, 'SparkMD5', {value: SparkMD5});

Vue.use(Meta);
new Vue({
    render: (h) => h(App),
}).$mount('#app');
