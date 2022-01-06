<template>
  <main class="wrapper">
    <nav class="navbar">
      <ul>
        <li>
          <a class="navigation-link" v-on:click="changeTab('control')">Кот</a>
        </li>
        <li>
          <a class="navigation-link" v-on:click="changeTab('settings')">Настройка</a>
        </li>
        <li>
          <a class="navigation-link" v-on:click="changeTab('update')">Обновление</a>
        </li>
      </ul>
    </nav>
    <section class="container" id="control">
      <br>
      <h2 style="text-align: center;">Управление котом</h2>
      <br>
      <h3>Яркость :</h3>
      <div class="row">
        <input class="column" type="range" id="brightness" name="brightness" v-on:change="saveLedConf()"
               v-model="ledConf.brightness" min="0" max="255" step="17">
      </div>
      <br>
      <h3>Режим :</h3>
      <div class="row">
        <div class="column column-20">
          <Button v-on:click="clickButton('<');" style="width: 100%"><h3><</h3></Button>
        </div>
        <div class="column column-60">
          <h3 style="text-align: center;" id="mode"> {{ ledConf.mode + 1 }} / {{ ledConf.paletteList.length }} </h3>
        </div>
        <div class="column column-20">
          <Button v-on:click="clickButton('>');" style="width: 100%"><h3>></h3></Button>
        </div>
      </div>
      <br>
    </section>
    <section class="container" id="settings" style="display: none">
      <h2>Настройка WiFi</h2>
      <div>Только латиница. Пароль должен быть длиннее 8 символов</div>
      <table id='apConf'>
        <thead>
        <tr>
          <th>SSID</th>
          <th>password</th>
          <th>enabled</th>
        </tr>
        </thead>
        <tbody>
        <tr>
          <td><input class="table-elem ssid" placeholder="loading" :value="wifiConf.ap.ssid"></td>
          <td><input class="table-elem password" placeholder="loading" :value="wifiConf.ap.pass"></td>
          <td>
            <div class="row">
              <div class='column'>
                <input class="table-elem active" type="checkbox">
              </div>
            </div>
          </td>
        </tr>
        </tbody>
      </table>
      <table id='coConf'>
        <thead>
        <tr>
          <th>SSID</th>
          <th>password</th>
          <th>action</th>
        </tr>
        </thead>
        <tbody>
        <tr v-for="(c,i) in wifiConf.co">
          <td class="ssid">{{ c.ssid }}</td>
          <td class="password">{{ c.pass }}</td>
          <td>
            <div class="row">
              <div class="column">
                <button class="table-elem up">ᐱ</button>
              </div>
              <div class="column">
                <button class="table-elem down">ᐯ</button>
              </div>
              <div class="column">
                <button class="table-elem delete" v-on:click="deleteConnection(i)">🗑</button>
              </div>
            </div>
          </td>
        </tr>
        <tr>
          <td><input class="table-elem" v-model="newConn.ssid"></td>
          <td><input class="table-elem" v-model="newConn.pass"></td>
          <td>
            <div class="row">
              <div class='column'>
                <Button class="table-elem" v-on:click="addConnection()">add</Button>
              </div>
            </div>
          </td>
        </tr>
        </tbody>
      </table>
      <Button v-on:click="saveWifiConf()">save</Button>
    </section>
    <section class="container" id="update" style="display: none">
      <fieldset id="code_fs">
        <h2>Update</h2>
        <div class="row">
          <div class="column">
            <input type="radio" id="firmware" name="type" value="firmware" v-model="ota.type">
            <label class="label-inline" for="firmware">firmware</label>
            <input class="label-inline" type="radio" id="filesystem" name="type" value="filesystem" v-model="ota.type">
            <label class="label-inline" for="filesystem">filesystem</label>
          </div>
        </div>
        <div class="row">
          <div class="column">
            <label class="file" style="width: 100%">
              <input id="file" type='file' name='file' accept=".bin,.bin.gz" @change="uploadOTA">
              <span class="file-custom"></span>
            </label>
            <div class="pt-2" v-if="!ota.uploading && ota.error !== null" key="error">
              <div>{{ ota.error }}</div>
              <button class="btn btn-primary ml-2" @click="retryOTA">Retry</button>
            </div>
            <div class="pt-2" v-else-if="ota.uploading">{{ ota.progress }}%</div>
            <div class="col-12 mt-3 pt-2 p-centered" v-else-if="!ota.uploading && ota.success" key="success">Success
            </div>
            <div v-else> Please chose file</div>
          </div>
        </div>
      </fieldset>
    </section>
  </main>
</template>

<script>
export default {
  name: 'smart cat',
  metaInfo: {
    title: '🐱 smart cat'
  },
  data() {
    return {
      ota: {
        uploading: false,
        progress: 0,
        error: null,
        success: false,

        type: 'firmware',
        file: null,
      },
      newConn: {
        ssid: null,
        pass: null
      },
      wifiConf: {
        apOn: true,
        ap: {
          ssid: "",
          pass: ""
        },
        co: []
      },
      ledConf: {
        mode: 1,
        brightness: 128,
        paletteList: []
      },
    };
  },

  methods: {
    saveLedConf: function () {
      fetch("/conf/led.json", {
        method: 'POST',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(this.ledConf)
      });
    },
    saveWifiConf: function () {
      fetch("/conf/wifi.json", {
        method: 'POST',
        headers: {
          'Accept': 'application/json',
          'Content-Type': 'application/json'
        },
        body: JSON.stringify(this.wifiConf)
      });
    },
    deleteConnection: function (i) {
      this.wifiConf.co.splice(i, 1)
    },
    addConnection: function () {
      this.wifiConf.co.push({ssid: this.newConn.ssid, pass: this.newConn.pass})
      this.newConn.ssid = null;
      this.newConn.pass = null;
    },
    changeTab: function (tabName) {
      let i, tabContent;
      tabContent = document.getElementsByClassName("container");
      for (i = 0; i < tabContent.length; i++) {
        tabContent[i].style.display = "none";
      }
      document.getElementById(tabName).style.display = "block";
    },
    clickButton: function (type) {
      if (type === '>' && this.ledConf.mode < this.ledConf.paletteList.length - 1) {
        this.ledConf.mode = this.ledConf.mode + 1;
      } else if (type === '<' && this.ledConf.mode > 0) {
        this.ledConf.mode = this.ledConf.mode - 1;
      } else return;
      this.saveLedConf()
    },
    fileMD5(file) {
      return new Promise((resolve, reject) => {
        const blobSlice = File.prototype.slice
            || File.prototype.mozSlice || File.prototype.webkitSlice;
        const chunkSize = 2097152; // Read in chunks of 2MB
        const chunks = Math.ceil(file.size / chunkSize);
        const spark = new this.SparkMD5.ArrayBuffer();
        const fileReader = new FileReader();
        let currentChunk = 0;
        let loadNext;

        fileReader.onload = (e) => {
          spark.append(e.target.result); // Append array buffer
          currentChunk += 1;

          if (currentChunk < chunks) {
            loadNext();
          } else {
            const md5 = spark.end();
            resolve(md5);
          }
        };

        fileReader.onerror = (e) => {
          reject(e);
        };

        loadNext = () => {
          const start = currentChunk * chunkSize;
          const end = ((start + chunkSize) >= file.size) ? file.size : start + chunkSize;

          fileReader.readAsArrayBuffer(blobSlice.call(file, start, end));
        };

        loadNext();
      });
    },
    uploadOTA(event) {
      this.ota.uploading = true;
      const formData = new FormData();
      if (event !== null) {
        [this.ota.file] = event.target.files;
      }
      const request = new XMLHttpRequest();
      request.addEventListener('load', () => {
        // request.response will hold the response from the server
        if (request.status === 200) {
          this.ota.success = true;
        } else if (request.status !== 500) {
          this.ota.error = `[HTTP ERROR] ${request.statusText}`;
        } else {
          this.ota.error = request.responseText;
        }
        this.ota.uploading = false;
        this.ota.progress = 0;
      });
      // Upload progress
      request.upload.addEventListener('progress', (e) => {
        this.ota.progress = Math.trunc((e.loaded / e.total) * 100);
      });
      this.fileMD5(this.ota.file)
          .then((md5) => {
            formData.append('MD5', md5);
            formData.append(this.ota.type, this.ota.file, this.ota.type);
            request.open('post', '/update');
            request.send(formData);
          })
          .catch(() => {
            this.error = 'Unknown error while upload, check the console for details.';
            this.uploading = false;
            this.progress = 0;
          });
    },
    retryOTA() {
      this.error = null;
      this.success = false;
      this.uploadOTA(null);
    },
    clear() {
      this.error = null;
      this.success = false;
    },
  },

  created() {
    fetch("/conf/led.json")
        .then(r => r.json())
        .then(d => this.ledConf = d);
    fetch("/conf/wifi.json")
        .then(r => r.json())
        .then(d => this.wifiConf = d);
  },
};
</script>

<style lang="css">
@import "~milligram";

.navbar {
  margin-bottom: 10px;
}

.navbar ul {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
  background-color: #606c76;
}

.navbar li {
  float: left;
  margin-bottom: 0;
}

.navbar li a {
  display: block;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
}

.navbar li a:hover {
  background-color: #ab5dda;
}

input:not([type]).table-elem {
  margin: 0;
  border: none;

  color: #606c76;
  font-family: 'Roboto', 'Helvetica Neue', 'Helvetica', 'Arial', sans-serif;
  font-size: 1em;
  font-weight: 300;
  letter-spacing: .01em;
  line-height: 1.6;
}

input[type='checkbox'].table-elem {
  margin: 0;
  border: none;
}

button.table-elem {
  margin: 0;
  width: 100%;
}
</style>
