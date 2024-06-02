// API 엔드포인트
const airQualityUrl = "http://openapi.seoul.go.kr:8088/52414a44616368653132364a67564550/xml/ListAirQualityByDistrictService/1/5/111121/";
const weatherUrl = "http://www.weather.go.kr/w/rss/dfs/hr1-forecast.do?zone=1114057000";

// 데이터를 저장할 배열
let airQualityData = [];
let weatherData = [];

// 차트를 초기화하는 함수
function initializeChart(canvasId, labels, data, label) {
    const ctx = document.getElementById(canvasId).getContext("2d");
    return new Chart(ctx, {
        type: "bar",
        data: {
            labels: labels,
            datasets: [{
                label: label,
                backgroundColor: "rgba(75, 192, 192, 0.2)",
                borderColor: "rgba(75, 192, 192, 1)",
                borderWidth: 1,
                data: data,
            }]
        },
        options: {
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero: true
                    }
                }]
            }
        }
    });
}

// API에서 데이터를 가져와서 차트를 업데이트하는 함수
async function updateCharts() {
    // 공기 질 데이터 가져오기
    const airQualityResponse = await fetch(airQualityUrl);
    const airQualityData = await airQualityResponse.text();

    // 날씨 데이터 가져오기
    const weatherResponse = await fetch(weatherUrl);
    const weatherData = await weatherResponse.text();

    // 차트 업데이트
    updateAirQualityChart(airQualityData);
    updateWeatherChart(weatherData);
}

// 공기 질 차트 업데이트 함수
function updateAirQualityChart(data) {
    // 데이터 파싱
    // TODO: 데이터 파싱 및 처리

    // 차트 업데이트
    // TODO: 차트 업데이트
}

// 날씨 차트 업데이트 함수
function updateWeatherChart(data) {
    // 데이터 파싱
    // TODO: 데이터 파싱 및 처리

    // 차트 업데이트
    // TODO: 차트 업데이트
}

// 페이지 로드시 차트 업데이트
window.onload = function () {
    updateCharts();
};
