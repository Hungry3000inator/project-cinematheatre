#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "MyForm.h"

using namespace std;

class CinemaBookingTest : public ::testing::Test {
protected:
    MyForm myForm;

    void SetUp() override {
        myForm = MyForm();
    }
};

// Column 0-2: Film does not exist -> Error "Film not found"
TEST_F(CinemaBookingTest, Column0_2_FilmNotFound) {
    string invalidFilm = "Non-existent film";

    bool isFilmValid = myForm.isFilmExists(invalidFilm);

    EXPECT_FALSE(isFilmValid) << "Film should not exist";
}

// Column 3: Session ended -> Error "Session already ended"
TEST_F(CinemaBookingTest, Column3_SessionEnded) {
    string validFilm = "Avatar";
    string expiredSession = "09:00";

    myForm.setFilm(validFilm);
    bool filmExists = myForm.isFilmExists(validFilm);
    bool sessionActive = myForm.isSessionActive(expiredSession);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_FALSE(sessionActive) << "Session should not be active";
}

// Column 4: Seats occupied -> Error "Seats unavailable"
TEST_F(CinemaBookingTest, Column4_SeatsUnavailable) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> bookedSeats = {1, 2, 3};

    myForm.setFilm(validFilm);
    myForm.setSession(activeSession);
    myForm.selectSeats(bookedSeats);

    bool filmExists = myForm.isFilmExists(validFilm);
    bool sessionActive = myForm.isSessionActive(activeSession);
    bool seatsAvailable = myForm.areSeatsAvailable(bookedSeats);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_FALSE(seatsAvailable) << "Seats should not be available";
}

// Column 5: User not authorized -> Error "User not authorized"
TEST_F(CinemaBookingTest, Column5_UserNotAuthorized) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};

    myForm.setFilm(validFilm);
    myForm.setSession(activeSession);
    myForm.selectSeats(freeSeats);

    bool filmExists = myForm.isFilmExists(validFilm);
    bool sessionActive = myForm.isSessionActive(activeSession);
    bool seatsAvailable = myForm.areSeatsAvailable(freeSeats);
    bool userAuthorized = myForm.isUserAuthorized();

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_FALSE(userAuthorized) << "User should not be authorized";
}

// Column 6: Insufficient funds -> Error "Insufficient funds"
TEST_F(CinemaBookingTest, Column6_InsufficientFunds) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};
    string authorizedUser = "user@example.com";
    double insufficientFunds = 50.0;

    myForm.authorizeUser(authorizedUser);
    myForm.setFilm(validFilm);
    myForm.setSession(activeSession);
    myForm.selectSeats(freeSeats);

    bool filmExists = myForm.isFilmExists(validFilm);
    bool sessionActive = myForm.isSessionActive(activeSession);
    bool seatsAvailable = myForm.areSeatsAvailable(freeSeats);
    bool userAuthorized = myForm.isUserAuthorized();
    bool hasFunds = myForm.hasSufficientFunds(insufficientFunds);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_TRUE(userAuthorized) << "User should be authorized";
    EXPECT_FALSE(hasFunds) << "Funds should be insufficient";
}

// Column 7: All conditions met -> Successful booking
TEST_F(CinemaBookingTest, Column7_SuccessfulBooking) {
    string validFilm = "Avatar";
    string activeSession = "18:00";
    vector<int> freeSeats = {10, 11, 12};
    string authorizedUser = "user@example.com";
    double sufficientFunds = 300.0;

    myForm.authorizeUser(authorizedUser);
    myForm.setFilm(validFilm);
    myForm.setSession(activeSession);
    myForm.selectSeats(freeSeats);

    bool filmExists = myForm.isFilmExists(validFilm);
    bool sessionActive = myForm.isSessionActive(activeSession);
    bool seatsAvailable = myForm.areSeatsAvailable(freeSeats);
    bool userAuthorized = myForm.isUserAuthorized();
    bool hasFunds = myForm.hasSufficientFunds(sufficientFunds);

    EXPECT_TRUE(filmExists) << "Film should exist";
    EXPECT_TRUE(sessionActive) << "Session should be active";
    EXPECT_TRUE(seatsAvailable) << "Seats should be available";
    EXPECT_TRUE(userAuthorized) << "User should be authorized";
    EXPECT_TRUE(hasFunds) << "Funds should be sufficient";

    bool bookingCreated = myForm.createBooking();
    EXPECT_TRUE(bookingCreated) << "Booking should be created";
}

// Test validation of all available films
TEST_F(CinemaBookingTest, AllFilmsAreValid) {
    vector<string> expectedFilms = {
        "Avatar",
        "Interstellar",
        "The Matrix",
        "Titanic"
    };

    for (const auto& film : expectedFilms) {
        EXPECT_TRUE(myForm.isFilmExists(film))
            << "Film '" << film << "' should be valid";
    }
}

// Test validation of all available sessions
TEST_F(CinemaBookingTest, AllSessionsAreValid) {
    vector<string> expectedSessions = {
        "10:00",
        "13:00",
        "16:00",
        "19:00"
    };

    string validFilm = "Avatar";
    myForm.setFilm(validFilm);

    for (const auto& session : expectedSessions) {
        EXPECT_TRUE(myForm.isSessionExists(session))
            << "Session '" << session << "' should exist";
    }
}

// Test ticket price calculation
TEST_F(CinemaBookingTest, TicketPriceCalculation) {
    string validFilm = "Avatar";
    vector<int> seats = {1, 2, 3};

    myForm.setFilm(validFilm);
    myForm.selectSeats(seats);

    double expectedPrice = 300.0;
    double actualPrice = myForm.calculateTotalPrice();

    EXPECT_DOUBLE_EQ(actualPrice, expectedPrice)
        << "Price should be calculated correctly";
}

// Test error when invalid seats are selected
TEST_F(CinemaBookingTest, InvalidSeatsReturnsError) {
    string validFilm = "Avatar";
    vector<int> invalidSeats = {-1, 100};

    myForm.setFilm(validFilm);
    
    EXPECT_FALSE(myForm.selectSeats(invalidSeats))
        << "Invalid seats should return error";
}

// Test error when invalid film is selected
TEST_F(CinemaBookingTest, InvalidFilmReturnsError) {
    string invalidFilm = "Non-existent film";
    
    EXPECT_FALSE(myForm.setFilm(invalidFilm))
        << "Invalid film should return error";
}
