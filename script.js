// Configura la conexión con Firebase
const firebaseConfig = {
    apiKey: "AIzaSyDelNZNxn62IZuo9RLVcZbtPCwuF2AquTM",
    authDomain: "ldr-lectura.firebaseapp.com",
    databaseURL: "https://ldr-lectura-default-rtdb.firebaseio.com",
    projectId: "ldr-lectura",
    storageBucket: "ldr-lectura.appspot.com",
    messagingSenderId: "1038838287922",
    appId: "1:1038838287922:web:378f5d6f0d759298f8362b"
};

// Inicializa Firebase
firebase.initializeApp(firebaseConfig);

var starCountRef = firebase.database().ref('LDR');
starCountRef.on('value', (snapshot) => {
  const data = snapshot.val()
  console.log(`El valor1 ha cambiado a ${data}`)
  var parrafo2=document.getElementById("valor-ldr");
  let porcentaje=100-Math.round((data*100)/1024);
  parrafo2.innerHTML=`${porcentaje}%`;

    // Mapea el rango de porcentajes a un rango de grados de rotación (por ejemplo, de 0 a 180 grados)
  let gradosRotacion = map(porcentaje, 0, 100, 0, 180);

    // Aplica la rotación al elemento con la clase "barraTemp"
    document.getElementById('grado').style.transform = `rotate(${gradosRotacion}deg)`;
});

function map(value, inMin, inMax, outMin, outMax) {
    return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
  }







