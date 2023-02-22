import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
from models.word import Word, Comment
from dash.dependencies import Input, Output
from termcolor import cprint
from shared import sort_by_y
import os

class Dashboard:
    def __init__(self, graph_words_title="", graph_words_type="bar", graph_timeline_type="line", data_name="Unknown") -> None:

        # preprocess data
        # init dash
        self.dashboard = dash.Dash(__name__)

        # design the html layout
        x, y = self.extract_dataset(0)
        x, y = sort_by_y(x, y)  # actually dataset is mostly sorted, thius line is just to make sure that data is always sorted

        self.dashboard.layout = self.create_layout(x, y)
        data_name = data_name.split('.')
        data_name = ''.join(data_name[:-1])
        self.data_name = data_name
        self.handle_limit_change_event(graph_words_title, graph_words_type)
        self.handle_timeline_word_select_event(x, y, graph_timeline_type)
        self.handle_save_table_event()

    def extract_dataset(self, limit):
        self.dataset = Word.most_used(limit=int(limit))
        if type(self.dataset) is dict:
            return (list(self.dataset.keys()), list(self.dataset.values()))
        return self.dataset


    def create_layout(self, x, y):
        tbody = []
        self.clicked_state = [0 for i in range(len(x))]
        for i in range(0, len(x), 2):
            tbody.append(html.Tr(children=[html.Td(), html.Td(html.Button(x[i], id=f"btn_{x[i]}", n_clicks=0, className="word-button")), html.Td(y[i]),
                                           html.Td(html.Button(x[i+1], id=f"btn_{x[i+1]}", n_clicks=0, className="word-button")), html.Td(y[i+1])]))


        return html.Div(
                    className="container",
                    children=[
                        html.Div(
                            className="title",
                            children=[
                                html.H1(children="Statistics & Timelines"),
                                html.P(children="Here we count all the hashtags that are used among the comments of post viewers:"),
                            ]
                        ),
                        html.Hr(className="separator"),
                        html.Div(
                            children=[
                                html.Div(children="Limit", className="limit-drop-down"),
                                dcc.Dropdown(
                                    id="limit-filter",
                                    options=[5, 10, 15, 20, 25, 30, 35, 40, 45, 50],
                                    value="10",
                                    clearable=False,
                                    className="limit-drop-down"
                                ),
                            ]
                        ),
                        dcc.Graph(id="words-chart"),
                        html.Hr(className="separator"),
                        html.H1(className="score-average", children=f"Score average is {Comment.average()}"),
                        dcc.Graph(id="timeline-chart", figure={
                                "data": [
                                    {
                                        "x": [],
                                        "y": [],
                                        "type": "line",
                                    },
                                ],
                                "layout": {"title": ""},
                            },
                        ),
                        html.Table(id="tableWords", className="table-words", children=[
                            html.Thead(
                                html.Tr(children=[html.Th(
                                    html.Button(id="btnSaveTable",className="download-button", children=html.I(className="fas fa-cloud-upload-alt"))
                                ), html.Th("Word"), html.Th("Iterations"), html.Th("Word"), html.Th("Iterations"), ]),
                            ),
                            html.Tbody(tbody)
                        ]),
                    ]
                )

    def handle_limit_change_event(self, graph_words_title, graph_words_type):
        @self.dashboard.callback(
            [Output("words-chart", "figure")],
            [Input("limit-filter", "value")]
        )
        def update_chart(limit):
            x, y = self.extract_dataset(limit)
            return {
                "data": [
                    {
                        "x": x,
                        "y": y,
                        "type": graph_words_type,
                    },
                ],
                "layout": {"title": graph_words_title},
            },

    def handle_timeline_word_select_event(self, x, y, graph_timeline_type):
        @self.dashboard.callback(
            [Output("timeline-chart", "figure")],
            [Input(f"btn_{x[i]}", "n_clicks") for i in range(len(x))],

        )
        def change_word_timeline(*clicks):
            for i, click in enumerate(clicks):
                if click and self.clicked_state[i] != clicks[i]:
                    self.clicked_state[i] = clicks[i]
                    dataset = Word.timeline(x[i])

                    return {
                        "data": [
                            {
                                "x": list(dataset.keys()),
                                "y": list(dataset.values()),
                                "type": graph_timeline_type,

                            }
                        ],
                        "layout": {"title": f"{x[i]}'s timeline"}
                    },

            return {
                        "data": [
                            {
                                "x": [],
                                "y": [],
                                "type": "line",
                            },
                        ],
                        "layout": {"title": "Select a word to show its timeline:"},
                    },

    def handle_save_table_event(self):
        @self.dashboard.callback(
            Output("btnSaveTable", "n_clicks"),
            [Input("btnSaveTable", "n_clicks")]
        )
        def save_table_as_excel(click):
            if self.dataset:
                data_frame = pd.DataFrame(data=self.dataset, index=["Word", "Iterations"])
                if not os.path.exists('excels'):
                    os.makedirs('excels')

                cprint(f"Successfully exported to excel as 'excels/{self.data_name}.xlsx'", "green")
                data_frame.T.to_excel(f"excels/{self.data_name}.xlsx")

    # now run the server
    def run(self, port = 8000, debug=False):
        self.dashboard.run_server("127.0.0.1", port, debug=debug)
