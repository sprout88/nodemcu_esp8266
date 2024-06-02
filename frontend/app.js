const express = require('express');
const axios = require('axios');
const path = require('path');
const xml2js = require('xml2js');

const app = express();

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/data', async (req, res) => {
    try {
        // 서울시 대기질 데이터 가져오기
        const airQualityResponse = await axios.get("http://openapi.seoul.go.kr:8088/52414a44616368653132364a67564550/xml/ListAirQualityByDistrictService/1/5/111121/");
        const airQualityData = airQualityResponse.data;

        // 기상청 날씨 데이터 가져오기
        const weatherResponse = await axios.get("http://www.weather.go.kr/w/rss/dfs/hr1-forecast.do?zone=1114057000");
        const weatherDataXml = weatherResponse.data;

        // XML 데이터를 JSON으로 변환
        const parser = new xml2js.Parser();
        const weatherData = await parser.parseStringPromise(weatherDataXml);

        // 데이터 전송
        res.json({ airQualityData, weatherData });
    } catch (error) {
        console.error("Error fetching data:", error);
        res.status(500).send("Error fetching data");
    }
});

const port = 3000;
app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}/`);
});
