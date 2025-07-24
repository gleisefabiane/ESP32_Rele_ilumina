//declaro os objetos - lâmpadas

let lampadaSalaLigada = false
let lampadaCozinhaLigada = false

let clienteWeb = null;

const clientId = 'Esp32' + Math.floor(Math.random() * 900) + 100;
clienteWeb = new Paho.MQTT.Client('broker.hivemq.com', 8884, clientId);

clienteWeb.connect({   
    useSSL: true, 
    onSuccess: function() {
        alert('A conexão com Broker foi bem sucedida')
    },
    onFailure: function() {
        alert('A conexão com Broker falhou')
    }
});


function ligarSala(){
    if (lampadaSalaLigada == false){
        document.getElementById("lamp-sala").classList.add("acesa")
        document.getElementById("bt-sala").textContent = "Desligar"
        lampadaSalaLigada = true
    }
    else{
        document.getElementById("lamp-sala").classList.remove("acesa")
        document.getElementById("bt-sala").textContent = "Ligar"
        lampadaSalaLigada = false
    }
}

function ligarCozinha()
{
    if (lampadaCozinhaLigada == false){
        lampadaCozinhaLigada = true
        document.getElementById("bt-cozinha").textContent = "Desligar"
    }
    else
    {
        lampadaCozinhaLigada = false
        document.getElementById("bt-cozinha").textContent = "Ligar"
    }
    document.getElementById("lamp-cozinha").classList.toggle("acesa")
 
}
