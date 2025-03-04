async function loadLeaderboard() {
    const response = await fetch("leaderboard.json");
    const data = await response.json();

    let leaderboardDiv = document.getElementById("leaderboard");
    leaderboardDiv.innerHTML = "";

    Object.keys(data)
        .sort((a, b) => data[b].win_percentage - data[a].win_percentage)
        .forEach(player => {
            let playerDiv = document.createElement("div");
            playerDiv.className = "player";

            // Image path (you can add images later)
            let imgPath = `images/${player}.jpg`;

            // Format best champions as bullet points
            let championsList = data[player].best_champions.map(champ => `<li>${champ}</li>`).join("");

            playerDiv.innerHTML = `
                <div class="player-container">
                    <img src="${imgPath}" alt="${player}" class="player-img" onerror="this.src='images/default.jpg'">
                    <div class="player-info">
                        <h2 class="player-name">${player}</h2>
                        <p>Win Percentage: ${data[player].win_percentage}</p>
                        <p>Best Teammate: ${data[player].best_teammate}</p>
                        <ul class="champ-list">${championsList}</ul>
                    </div>
                </div>
            `;
            leaderboardDiv.appendChild(playerDiv);
        });
}

loadLeaderboard();
