import folium
import json
import matplotlib.pyplot as plt

filepath = r"C:\Users\lauri\Desktop\microHorizon\build\src\microHorizonApp\output.json"
DRAW_MATPLOTLIB = True

def draw_folium(m, data):
    for candidate in data["candidates"]:
        if candidate["id"] == data["nearestRoadId"]:
            color = "#FF0000"
        else:
            color = "#0000FF"
        folium.PolyLine(
            locations=candidate["nodes"],
            color=color,
            weight=3,
            tooltip=f'ID: {candidate["id"]}, name: {candidate["name"]}'
        ).add_to(m)
    
    folium.Marker(
        location=[data["posLat"], data["posLon"]],
        tooltip="Input position",
        icon=folium.Icon(color="blue"),
    ).add_to(m)

def draw_matplotlib(data):
    for candidate in data["candidates"]:
        xs, ys = [], []
        for node in candidate["nodes"]:
            xs.append(node[0])
            ys.append(node[1])
        plt.plot(xs, ys)
    plt.plot(data["posLat"], data["posLon"], "ro")
    plt.show()

if __name__ == "__main__":
    with open(filepath, "r") as f:
        data = json.load(f)

    m = folium.Map(location=[data["posLat"], data["posLon"]], zoom_start=17)
    draw_folium(m, data)
    
    m.show_in_browser()

    if DRAW_MATPLOTLIB:
        draw_matplotlib(data)
    
    


