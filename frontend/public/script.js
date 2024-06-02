const ctx = document.getElementById('myChart').getContext('2d');

async function fetchData() {
    try {
        const response = await fetch('/data');
        const data = await response.json();

        // 서울시 대기질 데이터 처리
        const airQualityData = data.airQualityData.ListAirQualityByDistrictService;
        let airQualityLabels = [];
        let airQualityValues = [];
        
        if (airQualityData && airQualityData.row) {
            airQualityLabels = airQualityData.row.map(item => item.MSRSTE_NM);
            airQualityValues = airQualityData.row.map(item => parseFloat(item.PM10));
        } else {
            console.error('Air quality data is not in the expected format:', airQualityData);
        }

        // 기상청 날씨 데이터 처리
        const weatherData = data.weatherData.rss.channel[0].item[0].description[0].body[0].data;
        let weatherLabels = [];
        let weatherValues = [];

        if (weatherData) {
            weatherLabels = weatherData.map(item => item.hour[0]);
            weatherValues = weatherData.map(item => parseFloat(item.temp[0]));
        } else {
            console.error('Weather data is not in the expected format:', data.weatherData);
        }

        // 차트를 그립니다.
        const myChart = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: [...airQualityLabels, ...weatherLabels],
                datasets: [{
                    label: 'Air Quality (PM10)',
                    data: airQualityValues,
                    backgroundColor: 'rgba(255, 99, 132, 0.2)',
                    borderColor: 'rgba(255, 99, 132, 1)',
                    borderWidth: 1
                }, {
                    label: 'Weather (Temperature)',
                    data: weatherValues,
                    backgroundColor: 'rgba(54, 162, 235, 0.2)',
                    borderColor: 'rgba(54, 162, 235, 1)',
                    borderWidth: 1
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true
                    }
                }
            }
        });
    } catch (error) {
        console.error('Error fetching data:', error);
    }
}

window.onload = fetchData;
