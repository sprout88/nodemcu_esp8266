// app.js

const express = require('express');
const axios = require('axios');
const path = require('path');

const app = express();

// 정적 파일을 제공하기 위해 express.static 미들웨어를 사용합니다.
app.use(express.static(path.join(__dirname, 'public')));

// 기본 라우트 설정
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// 데이터 엔드포인트 설정
app.get('/data', async (req, res) => {
    try {
        // 공기질 데이터 가져오기
        const airQualityResponse = await axios.get("http://openapi.seoul.go.kr:8088/52414a44616368653132364a67564550/xml/ListAirQualityByDistrictService/1/5/111121/");
        const airQualityData = airQualityResponse.data;

        // 데이터 전송
        res.json({ airQualityData });
    } catch (error) {
        console.error("Error fetching data:", error);
        res.status(500).send("Error fetching data");
    }
});

// 서버를 시작합니다.
const port = 3000;
app.listen(port, () => {
    console.log(`Server is running at http://localhost:${port}/`);
});
