// public/script.js

const ctx = document.getElementById('myChart').getContext('2d');

async function fetchData() {
    try {
        // 서버에서 데이터를 가져옵니다.
        const response = await fetch('/data');
        const data = await response.json();

        // 데이터를 가공합니다. (이 예제에서는 그냥 사용합니다.)
        const labels = Object.keys(data.airQualityData);
        const values = Object.values(data.airQualityData);

        // 차트의 색상을 지정합니다.
        const backgroundColors = Array.from({ length: labels.length }, () => 'rgba(54, 162, 235, 0.2)');
        const borderColors = Array.from({ length: labels.length }, () => 'rgba(54, 162, 235, 1)');

        // 차트를 그립니다.
        const myChart = new Chart(ctx, {
            type: 'bar',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Air Quality',
                    data: values,
                    backgroundColor: backgroundColors,
                    borderColor: borderColors,
                    borderWidth: 1
                }]
            },
            options: {
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

// 페이지가 로드되면 fetchData 함수를 호출하여 데이터를 가져오고 차트를 그립니다.
window.onload = fetchData;
